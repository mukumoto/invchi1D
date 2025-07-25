# invchi1D

A C++ program to estimate correctors using a 1D approximation of the wavefield gradient.  

---

## Features

- RDFS filtering for 1D seismic wavefield gradient data  
- corrector inversion  

---

## Requirements

- C++17 compatible compiler  
- [Armadillo](https://arma.sourceforge.net/) C++ linear algebra library  

---

## Build Instructions

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_INSTALL_PREFIX=(...)

# Build and install the executable
cmake --build . --target install
```

The executable will be installed to:

```
(...)/bin/invchi1D
```

---

## Usage

```bash
invchi1D [input_data] [output_file] [parameter_file]
```

### Arguments

| Argument        | Description                                                                 |
|-----------------|-----------------------------------------------------------------------------|
| `input_data`    | Binary file containing raw wavefield gradient data (e.g. `data.bin`)        |
| `output_file`   | Output text file to save inverted correctors (e.g. `chi.txt`)              |
| `parameter_file`| Text file containing RDFS and data configuration parameters (see below)     |

---

## Parameter File Format

The parameter file should be a plain text file with one key-value pair per line:

```txt
fmax    60000
dx      5E-4
dt      1E-7
order   20
ntr     199
ns      6000
v0      800
```

### Parameter Descriptions

| Key        | Meaning                                          |
|------------|--------------------------------------------------|
| `fmax`     | Maximum frequency of wavefield gradient data              |
| `dx`       | channel interval [m]                   |
| `dt`       | sampling time interval [s]                  |
| `order`    | Filter order                       |
| `ntr`      | Number of  channels                        |
| `ns`       | Number of time samples                |
| `v0`       | Reference wave velocity [m/s]                   |

---

## Input Format

- The input data must be a **binary file** (`double` precision), containing `ns × ntr` values in **column-major** order.
- Each column corresponds to a single trace over time.

---

## Output Format

- The output file is a **plain text file** with `ntr` lines, each representing the corrector for one trace:

```txt
 4.9373428984427076e-01
 8.7608733316783083e-01
 7.5831283310900055e-01
 7.0582926085070496e-01
...
```


---

## Example
You can reproduce the right panel of Fig. 10 in Mukumoto et al. 2025 by following the procedure below.

1. Download `MDA_ASIRI1_28072021_50kHzP0000S0000_V.sg2` (or optionally SU format) from [https://doi.org/10.57745/05FHZU](https://doi.org/10.57745/05FHZU).

2. Apply finite-difference and lowpass filtering with a cut-off frequency of 60 kHz to the raw data, then generate the binary output file `data.bin`.

3. Run the program:

```bash
./build/bin/invchi1D data.bin chi.txt params.txt
```

With `params.txt`:

```txt
fmax    60000
dx      5E-4
dt      1E-7
order   20
ntr     199
ns      6000
v0      800
```

---

## Notes

- The input data must already represent spatial derivative data (e.g. ∂u/∂x).
- Preprocessing might be required (not included here).

---


## Author

Kota Mukumoto  
kotam@icm.csic.es
© 2025
