# Placement Insights Dashboard (React + C++ REST API)
Upgraded to an interview-ready stack:
- Premium React dashboard with charts and KPI cards
- C++ REST API for analytics (`GET /health`, `GET /kpis`)
- CSV-driven branch and placement insights
## Structure
- `frontend-react/` React + Vite dashboard
- `analytics-cpp/` C++ analytics API
- `dataset/` CSV data source
## Run C++ API
```bash
cd analytics-cpp
cmake -S . -B build
cmake --build build
./build/placement_analytics_api
```
## Run React Frontend
```bash
cd frontend-react
npm install
npm run dev
```
Frontend calls API on `http://localhost:8082`.
