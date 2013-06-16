#!/usr/bin/python
#
# Copyright (c) 2013 The Chromium OS Authors. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are
# met:
#
#    * Redistributions of source code must retain the above copyright
# notice, this list of conditions and the following disclaimer.
#    * Redistributions in binary form must reproduce the above
# copyright notice, this list of conditions and the following disclaimer
# in the documentation and/or other materials provided with the
# distribution.
#    * Neither the name of Google Inc. nor the names of its
# contributors may be used to endorse or promote products derived from
# this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
#
# Utility to test the behavior of TPM_SaveState
# Duncan Laurie <dlaurie@chromium.org>
#
# First send a TPM_SelfTest command and then attempt
# to send the TPM_SaveState command.  Measure the time
# it takes before the command is accepted.
#

import os
import struct
import time
import glob

found_devices = glob.glob("/dev/tpm*")

TPM_DEVICE = found_devices[0]
TPM_RETRY_MS = 100       # milliseconds between retry
TPM_MAX_RESPONSE = 4096  # max return size
TPM_ERROR_RETRY = 0x800  # TPM_RETRY
TPM_STRUCT_FMT = '>HII'  # u16 code, u32 length, u32 command/error

TPM_CODE_REQUEST = 0xc1  # valid request code
TPM_CODE_RESPONSE = 0xc4 # valid return code

TPM_CMD_SELFTEST = 0x50  # TPM_SelfTest
TPM_CMD_SAVESTATE = 0x98 # TPM_SaveState

class Tpm:
    '''Interface for talking to TPM kernel driver.'''

    def __init__(self):
        self._device_path = TPM_DEVICE

    def SendRecv(self, command):
        # open the TPM device
        fd = os.open(self._device_path, os.O_RDWR)
        # send the command
        os.write(fd, command)
        try:
            response = os.read(fd, TPM_MAX_RESPONSE)
            os.close(fd)
        except OSError:
            print "TPM error"
            return -1
        # verify the response
        code, length, error = struct.unpack(TPM_STRUCT_FMT, response)
        # check for valid response code
        if code != TPM_CODE_RESPONSE:
            print 'TPM invalid response'
            return -1
        # check for errors
        if error != 0 and error != TPM_ERROR_RETRY:
            print 'TPM error 0x%x' % error
        return error

    def BuildCommand(self, code, length):
        return struct.pack(TPM_STRUCT_FMT, TPM_CODE_REQUEST,
                           length, code)

    def SelfTest(self):
        error = self.SendRecv(self.BuildCommand(TPM_CMD_SELFTEST, 10))

    def SaveState(self):
        command = self.BuildCommand(TPM_CMD_SAVESTATE, 10)
        wait_ms = 0
        while self.SendRecv(command) == TPM_ERROR_RETRY:
            wait_ms += TPM_RETRY_MS
            time.sleep(float(TPM_RETRY_MS / 1000.0))
        if wait_ms > 0:
            print "TPM_SaveState took %dms to respond" % wait_ms

if __name__ == '__main__':
    tpm = Tpm()
    tpm.SelfTest()
    tpm.SaveState()
