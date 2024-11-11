# Memory Accounting

## Empty project (plus SPI and RadioHead)

```
Program:    3366 bytes (10.3% Full)
(.text + .data + .bootloader)

Data:        149 bytes (5.8% Full)
(.data + .bss + .noinit)
```

--
## Bytes added by `RF_95` variable

```
Program:    9302 bytes (28.4% Full)
(.text + .data + .bootloader)

Data:        582 bytes (22.7% Full)
(.data + .bss + .noinit)
```

5936 = 9302 - 3366

### Discussion
6000 bytes for one variable? Some optimization is happening in the previous project to rip out the unused code. Which is good! But uhhh not actually as helpful as I'd hoped

### Bytes added by a second `RF_95` variable

```
Program:    9450 bytes (28.8% Full)
(.text + .data + .bootloader)

Data:        874 bytes (34.1% Full)
(.data + .bss + .noinit)
```

### Discussion
`RH_RF96` object itself only takes up 148 bytes. Adding the first one also adds (9302 - 3366 - 148 = ) 6048 bytes.

---

## Bytes added by `Serial.println()`
```
Program:    9534 bytes (29.1% Full)
(.text + .data + .bootloader)

Data:        876 bytes (34.2% Full)
(.data + .bss + .noinit)
```

9534 - 9450 = 84

## Bytes added by `snprintf()`
```
Program:   11206 bytes (34.2% Full)
(.text + .data + .bootloader)

Data:        912 bytes (35.6% Full)
(.data + .bss + .noinit)
```

1672 = 11206 - 9534

## Our Project

```
Program:   14838 bytes (45.3% Full)
(.text + .data + .bootloader)

Data:       1170 bytes (45.7% Full)
(.data + .bss + .noinit)
```

Absolutely required elements:
- RF_95: 6048 bytes
- Firmware: 3366 bytes

Total: 9414

Known removable libraries:
- printf() and friends: 1672 bytes

Total external stuff, both optional and required: 11086

Maximum additional usage by TDM code:
- 3752 bytes

Size on flash removing known optional stuff:
- 13166 bytes

### Discussion
The absolute most amount of space my TDM protocol code could be taking up is 3752 bytes. That said, it's dependent on 9414 bytes worth of stuff. So, removing the optional stuff will result in a predicted total size of 13166 bytes.

## Our Project (non-gateway)
```
Program:   15070 bytes (46.0% Full)
(.text + .data + .bootloader)

Data:        935 bytes (36.5% Full)
(.data + .bss + .noinit)
```