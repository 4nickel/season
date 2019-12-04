#### Calculate the date and time of solstices and equinoctes.
###### Based on "Astronomical Algorithms" by Jean Meeus.

The calculated dates and times are astronomical. Note that your official calendar seasons are subject to local regulation.

###### Usage:
```meeus-season [spring|summer|autumn|winter] year```

###### Installation:
```sh
git clone https://github.com/4nickel/meeus-season
cd meeus-season
make
```
Find the binary in ```bin``` and copy it to your ```$PATH```:
```sh
cp bin/meeus-season ~/.local/bin
```

###### Examples:
```sh
$ meeus-season spring 2019
2019-03-20@21:57
````
