# Arguments
```
-help             : Prints this message.
-pack [directory] : Packs all files (regardless of extension or type) into a texpack file.
	Note: This is not a recursive function. Only files directly within the given directory will be packed.
-unpack [texpack] : Unpacks all files from a texpack file.

	Resulting files are placed in an /output/ directory relative to this executable's location.
```

# Example
```
texpack -pack "directory goes here"
```
