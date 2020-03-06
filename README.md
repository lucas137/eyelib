_______________________________________________________________________________
# eyelib
Eye tracking C++ library  
https://github.com/lucas137/eyelib

### Directory structure
- `bin ................` binaries
- `cb .................` Code::Blocks projects
    - `calib ..........` Calibration app
    - `datalog ........` Data logging app
    - `eyelib .........` Eyelib static library
        - `doxygen ....` Doxygen configuration
    - `window .........` Gaze data dispaly app
- `doc ................` documentation
- `include ............` library interface headers
- `lib ................` static library
- `src ................` source code
- `test ...............` unit tests

### Modules
- `calib` -- Eye tracker calibration results (`calibration.hpp`)
  - Overall calibration state and error
  - Calibration point state, coordinates, error, and sample standard deviation
- `gaze` -- Eye gaze data and processing (`gaze.hpp`)
  - Gaze data
    - Timestamp
    - Tracking state
    - Fixation flag
    - Raw gaze point coordinates
    - Smoothed gaze point coordinates
    - Pupil coordinates and size
  - Fixation detection algorithms
  - Eye tracking metrics
    - Blink time
    - Fixation time
    - Saccade distance
    - Pupillometry
  - Point cluster
    - Group of gaze point coordinates
- `screen` -- Screen-related data structures and functions (`screen.hpp`)
  - 2D point
  - Gaze target point
  - Gaze target duration
  - RGB color
  - Screen parameters
- `tracker` -- Core eye tracking functionality (`tracker.hpp`)
  - Eye tracker device management
  - Callback registration
  - Calibration procedure and window
  - Optional window to display gaze points and calibration results

_______________________________________________________________________________
