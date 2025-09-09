import argparse
import pandas as pd
from sklearn.model_selection import train_test_split
from sklearn.impute import SimpleImputer
from lazypredict.Supervised import LazyClassifier
from sklearn.ensemble import RandomForestClassifier
import matplotlib.pyplot as plt

# Parsing command line arguments
parser = argparse.ArgumentParser(description='Predict purification mode (Chlorine/UV) from water quality data.')
parser.add_argument('--file_path', type=str, required=True, help='Path to your CSV file')
args = parser.parse_args()

# Load dataset
data = pd.read_csv(args.file_path)

# --- Feature Engineering with WHO/CDC thresholds ---
def assign_mode(row):
    if row["HumanPresence"] == 1:
        return "Chlorine"
    else:
        if row["ecoli"] > 0:
            return "Chlorine"
        elif row["turb"] > 5:
            return "Chlorine"
        elif not (6.5 <= row["ph"] <= 8.0):
            return "Chlorine"
        elif row["w_t"] > 25:
            return "Chlorine"
        else:
            return "UV"

# Apply rules
data["Mode"] = data.apply(assign_mode, axis=1)

# Select only your features
features = ["ecoli", "ph", "do", "flow", "a_t", "w_t", "turb", "HumanPresence"]
X = data[features]
y = data["Mode"]

# Impute missing numeric values
numerical_imputer = SimpleImputer(strategy="mean")
X[X.columns] = numerical_imputer.fit_transform(X)

# Train-test split
X_train, X_test, y_train, y_test = train_test_split(
    X, y, test_size=0.2, random_state=42
)

# --- Run LazyClassifier Benchmark ---
clf = LazyClassifier(verbose=0, ignore_warnings=True, predictions=True)
models, predictions = clf.fit(X_train, X_test, y_train, y_test)

print("Model Performance:")
print(models)
print("\nSample Predictions:")
print(predictions.head())

# --- Train RandomForest for Feature Importance ---
rf = RandomForestClassifier(n_estimators=100, random_state=42)
rf.fit(X_train, y_train)

importances = rf.feature_importances_
feature_importance_df = pd.DataFrame({
    "Feature": features,
    "Importance": importances
}).sort_values(by="Importance", ascending=False)

print("\nFeature Importance:")
print(feature_importance_df)

# --- Plot Feature Importance ---
plt.figure(figsize=(8, 5))
plt.barh(feature_importance_df["Feature"], feature_importance_df["Importance"])
plt.gca().invert_yaxis()
plt.title("Feature Importance for Chlorine vs UV Decision")
plt.xlabel("Importance Score")
plt.show()
