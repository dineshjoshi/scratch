README
------

These are a set of hacks I found over the Internet to get Google's
Chromebook Pixel working with Linux. Some of the scripts are
customized for my environment so they may need to be changed to fit
your specific paths.

Here is a list of hacks:

1. ACPI Call - required to make arbitrary acpi calls. Known to work
with Linux kernel 3.9 and lower.  

2. tpm_savestate_test.py - Required to make the sleep functionality
work correctly. This works in conjunction with the tpm_tis module.

3. rc.local - For whatever reason loading modules via /etc/modules
doesn't work correctly for tpm_tis as well as acpi_call. Hence the
work around is to use this file.

4. 10_turn-on-bl - This script runs whenever your machine wakes up
from sleep. It turns on the keyboard backlight. This can be customized
further to be intelligent enough to detect the state of the keyboard
backlight and set it appropriately.

Credits: 

1. Brock Tice's blog -
http://blog.brocktice.com/2013/03/09/running-debian-wheezy-7-0-on-the-chromebook-pixel/

2. Comment by 'B. A.' on Brock's blog about acpi_call for keyboard backlighting.

3. Miscellaneous bugs and G+ comments over the Internet for comments.

Please let me know if I have missed crediting you. Drop me an email
and I will update this with the appropriate credits. None of the code
/ scripts are original and are derivative works of others.