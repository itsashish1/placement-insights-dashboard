import React, { useEffect, useMemo, useState } from "react";
import { createRoot } from "react-dom/client";
import { BarChart, Bar, XAxis, YAxis, Tooltip, ResponsiveContainer, CartesianGrid } from "recharts";
function App() {
  const [api, setApi] = useState(null);
  const [error, setError] = useState("");
  useEffect(() => {
    fetch("http://localhost:8082/kpis")
      .then((r) => {
        if (!r.ok) throw new Error("API unavailable");
        return r.json();
      })
      .then(setApi)
      .catch(() => setError("C++ API not running. Showing fallback values."));
  }, []);
  const data = useMemo(() => api?.branchStats || [
    { branch: "CSE", offerRatio: 90 },
    { branch: "IT", offerRatio: 78 },
    { branch: "ECE", offerRatio: 63 },
    { branch: "ME", offerRatio: 40 },
    { branch: "EE", offerRatio: 35 }
  ], [api]);
  const students = api?.students ?? 10;
  const offerRatio = Number(api?.offerRatio ?? 70).toFixed(1);
  const avgCtc = Number(api?.avgHighestCtc ?? 8.6).toFixed(1);
  return (
    <div style={{
      minHeight: "100vh",
      padding: 22,
      fontFamily: "'Trebuchet MS', 'Segoe UI', sans-serif",
      background: "linear-gradient(135deg, #fef3c7 0%, #dbeafe 45%, #dcfce7 100%)"
    }}>
      <div style={{ maxWidth: 1120, margin: "0 auto" }}>
        <h1 style={{ marginBottom: 8, fontSize: 40 }}>Placement Insights Dashboard</h1>
        <p style={{ marginTop: 0 }}>Premium React analytics UI powered by C++ REST endpoints.</p>
        {error && <p style={{ color: "#b91c1c", fontWeight: 600 }}>{error}</p>}
        <div style={{ display: "grid", gridTemplateColumns: "repeat(3,minmax(180px,1fr))", gap: 12, margin: "16px 0" }}>
          <Kpi title="Students" value={students} />
          <Kpi title="Offer Ratio" value={`${offerRatio}%`} />
          <Kpi title="Avg Highest CTC" value={`${avgCtc} LPA`} />
        </div>
        <div style={{ background: "rgba(255,255,255,0.9)", borderRadius: 18, padding: 16, boxShadow: "0 8px 30px rgba(0,0,0,0.12)" }}>
          <h3 style={{ marginTop: 0 }}>Branch-wise Offer Ratio</h3>
          <div style={{ width: "100%", height: 340 }}>
            <ResponsiveContainer>
              <BarChart data={data}>
                <CartesianGrid strokeDasharray="3 3" />
                <XAxis dataKey="branch" />
                <YAxis />
                <Tooltip />
                <Bar dataKey="offerRatio" fill="#2563eb" radius={[8, 8, 0, 0]} />
              </BarChart>
            </ResponsiveContainer>
          </div>
        </div>
      </div>
    </div>
  );
}
function Kpi({ title, value }) {
  return (
    <div style={{ background: "white", borderRadius: 14, padding: 14, boxShadow: "0 6px 20px rgba(0,0,0,0.08)" }}>
      <div style={{ fontSize: 13, color: "#475569" }}>{title}</div>
      <div style={{ fontSize: 30, fontWeight: 800 }}>{value}</div>
    </div>
  );
}
createRoot(document.getElementById("root")).render(<App />);
