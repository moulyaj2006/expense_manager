#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <limits>

using namespace std;

struct Expense {
    string desc;
    int amount;
    string date;
};

int main() {

    vector<Expense> expenses;
    string desc, date;
    int amount;
    char choice;

    do {
        cout << "\nEnter Expense Description: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, desc);

        cout << "Enter Amount: ";
        cin >> amount;

        cout << "Enter Date (YYYY-MM-DD): ";
        cin >> date;

        expenses.push_back({desc, amount, date});

        cout << "Enter y to add more, any other key to stop: ";
        cin >> choice;

    } while (choice == 'y' || choice == 'Y');

    int total = 0;
    map<string, int> dailyTotals;

    for (auto &e : expenses) {
        total += e.amount;
        dailyTotals[e.date] += e.amount;
    }

    ofstream file("expense_manager.html");

    file << R"(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<title>Expense Manager Tracker</title>
<script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
<style>
body { font-family: Arial, sans-serif; background:#f4f6f8; margin:0; }
header { background:#2c3e50; color:white; padding:15px; text-align:center; }
.container { max-width:550px; margin:20px auto; background:white; padding:20px;
border-radius:8px; box-shadow:0 0 10px rgba(0,0,0,0.1); }
input, button { width:100%; padding:10px; margin:6px 0; }
button { background:#27ae60; color:white; border:none; }
ul { list-style:none; padding:0; }
li { background:#ecf0f1; margin:5px 0; padding:8px; border-radius:4px; }
.total { text-align:center; font-weight:bold; margin-top:10px; }
#chartBox { display:none; margin-top:20px; }
</style>
</head>
<body>

<header>
<h1>Expense Manager Tracker</h1>
<p>Track daily & weekly expenses</p>
</header>

<div class="container">

<ul>
)";

    for (auto &e : expenses) {
        file << "<li>" << e.date << " | " << e.desc << " | ₹" << e.amount << "</li>\n";
    }

    file << R"(</ul>

<div class="total">
Total Expense: ₹ )" << total << R"(
</div>

<button onclick="showGraph()">Show Expense Graph</button>
<div id="chartBox">
<canvas id="expenseChart"></canvas>
</div>

</div>

<script>
const labels = [)";

    bool first = true;
    for (auto &p : dailyTotals) {
        if (!first) file << ",";
        file << "\"" << p.first << "\"";
        first = false;
    }

    file << "];\nconst data = [";

    first = true;
    for (auto &p : dailyTotals) {
        if (!first) file << ",";
        file << p.second;
        first = false;
    }

    file << R"(];

function showGraph() {
    document.getElementById("chartBox").style.display = "block";
    new Chart(document.getElementById("expenseChart"), {
        type: "bar",
        data: {
            labels: labels,
            datasets: [{
                label: "Daily Expense",
                data: data,
                backgroundColor: "#3498db"
            }]
        },
        options: {
            responsive: true,
            scales: {
                y: { beginAtZero: true }
            }
        }
    });
}
</script>

</body>
</html>
)";

    file.close();

    cout << "\nHTML file generated: expense_manager.html\n";
    system("start expense_manager.html");

    return 0;
}
