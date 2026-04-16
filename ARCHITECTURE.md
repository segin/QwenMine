# QwenMine Architecture

## Project Structure

```
qwenmine/
├── backend/              # Game logic and state management
│   ├── Game.cpp
│   ├── Game.h
│   ├── Board.cpp
│   ├── Board.h
│   └── Cell.h
├── frontend/             # Qt UI components
│   ├── GameWindow.cpp
│   ├── GameWindow.h
│   ├── BoardView.cpp
│   └── BoardView.h
├── src/
│   └── main.cpp
├── CMakeLists.txt
└── ARCHITECTURE.md
```

The project follows a Model-View separation pattern where the backend handles game state and the frontend manages UI rendering and user interaction.

## High-Level System Diagram

```
┌─────────────────────────────────────────────────────┐
│                    GameWindow                        │
│              (Main Qt Window)                        │
│  ┌─────────────────────────────┐                   │
│  │    Difficulty Selection     │                   │
│  │    [Easy] [Medium] [Hard]   │                   │
│  └─────────────────────────────┘                   │
└─────────────────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────┐
│                    BoardView                        │
│              (Custom QWidget)                       │
│         ┌──────────────────┐                        │
│         │  Minesweeper Grid│                        │
│         │   (QPainter)     │                        │
│         └──────────────────┘                        │
└─────────────────────────────────────────────────────┘
                    │
                    │ Signals/Slots
                    │
┌─────────────────────────────────────────────────────┐
│                    Game                             │
│              (Game State Manager)                   │
│  ┌─────────────┐  ┌─────────────┐                  │
│  │  Game       │  │   Board     │                  │
│  │  Status     │  │  Manager    │                  │
│  │  (Playing)  │  │  (Grid)     │                  │
│  └─────────────┘  └─────────────┘                  │
│         │                 │                        │
│         ▼                 ▼                        │
│  ┌─────────────┐  ┌─────────────┐                 │
│  │  Cell       │  │  Game       │                 │
│  │  (Cell)     │  │  Signals    │                 │
│  └─────────────┘  └─────────────┘                 │
└─────────────────────────────────────────────────────┘
```

## Core Components

### Frontend

#### GameWindow
- **Type:** Main Qt Window (`QMainWindow`)
- **Responsibilities:**
  - Hosts the UI including `BoardView` and `QComboBox` for difficulty selection
  - Connects difficulty selection to game initialization
  - Displays game status and controls

#### BoardView
- **Type:** Custom Widget (`QWidget`)
- **Responsibilities:**
  - Renders the game board using `QPainter`
  - Responds to mouse events (clicks, right-click for flags)
  - Visual feedback for game states (win, loss)
  - Handles cell interaction through paint and event callbacks

### Backend

#### Game
- **Type:** QObject
- **Responsibilities:**
  - Manages game state (`Playing`, `Won`, `Lost`)
  - Coordinates between `GameWindow` and `Board`
  - Emits signals for UI updates and state changes
  - Handles game rules and win/loss conditions

#### Board
- **Type:** Data Management Class
- **Responsibilities:**
  - Manages the grid of `Cell` objects
  - Handles mine placement
  - Calculates adjacent mine counts
  - Validates moves and cell states

#### Cell
- **Type:** Data Structure
- **Responsibilities:**
  - Stores individual cell state:
    - `mine` (bool) - whether cell contains a mine
    - `flagged` (bool) - whether cell is marked with a flag
    - `revealed` (bool) - whether cell is shown to user
  - Provides state transition logic for revealed/flagged states

## Development Environment

- **Language:** C++
- **Framework:** Qt6 (Widgets, Core)
- **Build System:** CMake
- **Key Qt Components:**
  - `QMainWindow` - Main window container
  - `QWidget` - Base widget class
  - `QPainter` - 2D graphics rendering
  - `QObject` - Signal/slot mechanism
- **CMake Configuration:** Qt6 with required modules and include paths

## Future Considerations

- **Potential Enhancements:**
  - Timer functionality
  - Mine counter
  - Restart/Reset functionality
  - Multiple game instances
  - Sound effects
  - Animations for cell reveal

- **Architecture Evolution:**
  - Consider implementing Observer pattern for state notifications
  - Introduce GameSettings class for difficulty configuration
  - Add persistence layer for high scores
  - Consider separating rendering logic into dedicated renderer

## Glossary

- **GameWindow:** Main Qt window hosting the UI components
- **BoardView:** Custom widget that paints the game board
- **Game:** Central game state manager using Qt's signal/slot system
- **Board:** Grid management and mine placement logic
- **Cell:** Individual game board element with state flags
- **QPainter:** Qt class for 2D graphics rendering
- **Signal/Slot:** Qt mechanism for communication between objects