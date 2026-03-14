import pandas as pd
import streamlit as st
from sklearn.linear_model import LogisticRegression
st.set_page_config(page_title='Placement Insights Dashboard', layout='wide')
st.title('Placement Insights Dashboard')
df = pd.read_csv('data/sample_placements.csv')
offered = (df['offers'] > 0).astype(int)
c1, c2, c3 = st.columns(3)
c1.metric('Students', len(df))
c2.metric('Offer Ratio', f"{offered.mean()*100:.1f}%")
c3.metric('Avg Highest CTC', f"{df['highest_ctc_lpa'].mean():.1f} LPA")
st.subheader('Branch-wise Offer Ratio')
branch_stats = df.assign(placed=offered).groupby('branch')['placed'].mean().sort_values(ascending=False)
st.bar_chart(branch_stats)
st.subheader('Offer Probability Model')
X = df[['cgpa', 'internships', 'projects', 'aptitude_score']]
y = offered
model = LogisticRegression(max_iter=1000)
model.fit(X, y)
with st.form('predict_form'):
  st.write('Predict placement chance')
  cgpa = st.slider('CGPA', 5.0, 10.0, 8.0, 0.1)
  internships = st.slider('Internships', 0, 5, 1)
  projects = st.slider('Projects', 0, 8, 3)
  aptitude = st.slider('Aptitude Score', 40, 100, 75)
  run = st.form_submit_button('Predict')
if run:
  prob = model.predict_proba([[cgpa, internships, projects, aptitude]])[0][1] * 100
  st.success(f'Estimated placement probability: {prob:.1f}%')
  if prob < 60:
    st.info('Improve aptitude score and add project depth with measurable outcomes.')
