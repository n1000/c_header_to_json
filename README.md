# cparse

This script takes as input a preprocessed C header file with structure and enum
definitions and generates C code to print those structures out as JSON.

## Dependencies

This script makes user of the pycparser python C parsing library:

```console
python3 -m pip install pycparser
```
