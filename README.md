# BEKcracker

BEKcracker is a password cracking tool for storage devices encrypted with BitLocker, an encryption feature available on Windows Vista, 7, 8.1 and 10 (Ultimate, Pro and Enterprise editions), using USB Key Mode.

BEKcracker tries to find the correct USB Key (saved on a BEK file) to decrypt the encrypted storage device.

## Build

Running the `build.sh` script generates an executable inside the `build` directory: `BEKcracker`.

## BEK attack!

Run BEKcracker typing `./BEKcracker` on command line. To have the best performance two options are available:
- `-g` : specify GUID of your BEK file, with or without separators
- `-e` : specify year, month, day and hour of encryption following this format: yyyy mm dd hh (must be > <2007 01 07 00>)
  
For all the options, type `./BEKcracker -h`.
  
An example:
`./BEKcracker -g 70FE1FBE-B26B-43B8-BF26-B1C552FC8C34 -e 2020 08 30 12`

## References, credits and contacts

Plase share and test this project: we need your feedback! 

Special thanks to [BitCracker](https://github.com/e-ago/bitcracker), [Dislocker](https://github.com/Aorimn/dislocker) and [LibBDE](https://github.com/libyal/libbde) projects.
For any additional info, collaborations or bug report please contact me (emanuelecavalleri at email dot com) or open an issue.
