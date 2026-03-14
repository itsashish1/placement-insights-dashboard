import React from "react";
import { createRoot } from "react-dom/client";
import { BarChart, Bar, XAxis, YAxis, Tooltip, ResponsiveContainer } from "recharts";
const data = [
  { branch: "CSE", offerRatio: 92 },
  { branch: "IT", offerRatio: 80 },
  { branch: "ECE", offerRatio: 65 },
  { branch: "ME", offerRatio: 45 },
  { branch: "EE", offerRatio: 40 }
];
function App() {
  return (
    <div style={{ fontFamily: "Segoe UI", padding: 24 }}>
      <h1>Placement Insights Dashboard (React + C++)</h1>
      <p>Interactive frontend in React. Core analytics available in C++ under analytics-cpp.</p>
      <div style={{ display: "grid", gridTemplateColumns: "repeat(3, minmax(180px, 1fr))", gap: 12, marginBottom: 20 }}>
        <div style={{ background: "#f3f4f6", padding: 12, borderRadius: 8 }}>Students: 10</div>
        <div style={{ background: "#f3f4f6", padding: 12, borderRadius: 8 }}>Offer Ratio: 70%</div>
        <div style={{ background: "#f3f4f6", padding: 12, borderRadius: 8 }}>Avg Highest CTC: 8.6 LPA</div>
      </div>
      <div style={{ width: "100%", height: 320 }}>
        <ResponsiveContainer>
          <BarChart data={data}>
            <XAxis dataKey="branch" />
            <YAxis />
            <Tooltip />
            <Bar dataKey="offerRatio" fill="#2563eb" />
          </BarChart>
        </ResponsiveContainer>
      </div>
    </div>
  );
}
createRoot(document.getElementById("root")).render(<App />);
