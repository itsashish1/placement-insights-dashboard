# Placement Insights Dashboard (React + C++)
This repository has been upgraded from Python Streamlit to:
- React dashboard UI (interactive charts)
- C++ analytics engine for KPI computation
## Structure
- `frontend-react/` React + Recharts UI
- `analytics-cpp/` C++17 metrics computation
- `dataset/` placement sample data
## Run React App
```bash
cd frontend-react
npm install
npm run dev
```
## Build C++ Analytics
```bash
cd analytics-cpp
cmake -S . -B build
cmake --build build
./build/placement_analytics
```
