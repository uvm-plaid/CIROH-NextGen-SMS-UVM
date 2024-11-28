Author: Jordan Bourdeau

Date: 11/6/2024

# Mayfly Code

This directory contains all the code related to the Mayfly for use with the
RockBLOCK9603. The directory is organized as a PlatformIO project, and the main
locations are as follows:

- `src/`
  - Directory where all the source code (.cpp and .c) files go.
- `include/`
  - Directory where header files should go. This directory contains definitions
    for classes, and is also the home to the ported model code.

## Porting sklearn Models

### Creating the Model

Models can be ported from sklearn using the `micromlgen` Python library. This
library is no longer supported, but is compatible with most of the sklearn
model types. An example usage can be found in the `micromlgen_port.py` script
in the `rockblock/models/` subdirectory. This example trains a model and exports
it to a C++ representation. The produced files are what should be placed in the
`include/` directory to be imported by the Arduino program.

NOTE: There is also an example using the `everywhereml` library which is newer
and receiving updates, but as of the time this is being written does not yet
fully support all the sklearn models under consideration.

### Importing the Model

Once the model has been produced and moved into the `include/` directory, the
import at the top of the file must be changed from

`#include <cstdarg>`

to

```
#include <Arduino.h>
#include <stdarg.h>
```

This is a necessary step for porting the C++ representation to the Arduino.
Once this step is complete, the model can then be declared in the source code
as follows:

`Eloquent::ML::Port::<ClassName> model;`

Where `<ClassName>` and `model` refer to the arbitrary names given to the
class declared in the ported header file and the variable through which the
interface is provided.

Once done, the program should be able to compile and deploy to the Arduino.

### Using the Model

Using the model is as simple as passing in a pointer to an array of arguments
(in the case of MFCCs, this will be an array of float values equal in length
to the number of MFCCs expected in the model). Be careful that this array is
properly constructed and is of the right length, as C++ does not enforce
bounds checks and would silently fail if it used garbage data leftover in the
array or from beyond the array bounds.
