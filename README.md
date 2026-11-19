# DecLib with Romana DEC parameter support

Includes the EOF-fixed event decoder, and the supplied `toptions.h` / `common.h` structures.

## Using the options

```cpp
DecFile file;
if (!file.Open("run.dec")) return;
const RomanaOptions &p = file.ROptions;
if (p.HasTOption("Period")) std::cout << p.TOptions.Period << " ns\n";
if (p.HasCOption("F_start")) std::cout << p.COptions.F_start << "\n";
nlohmann::json value;
if (p.TryGetTOption("E0", value)) std::cout << value.dump() << "\n";
// jsTOptions / jsCOptions contain only fields actually present in the file.
```

All plain fields listed in the supplied binary `BinRead.cpp` are mapped to the `COptions`/`TOptions` structs (including `device` and `ver_po`). Unknown fields are skipped; missing fields retain value-initialized defaults but must be tested with `Has*Option()`. Larger-than-declared fields, malformed lengths and incomplete scalar/array elements are rejected rather than corrupting nearby memory. Arrays shorter than their declared capacity are accepted with zero-filled tails. Duplicate fields use their last valid value.

Nontrivial ROOT `Hdef`/`TList` members are not decoded by the supplied BinRead mapping and require a separate on-disk definition. `TryGet*Option` covers mapped JSON-compatible values; the original typed structs remain available.

Compile/load with ROOT, zlib and nlohmann/json. `DecLibTest.cpp` includes `romana_headers/BinRead.cpp`, so do not compile the latter as an independent translation unit at the same time. Integration with an actual `.dec` file has not been tested in this environment.
