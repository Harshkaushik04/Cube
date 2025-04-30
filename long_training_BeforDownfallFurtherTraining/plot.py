import sys
import pandas as pd
import matplotlib.pyplot as plt

def generate_plot(csv_file, output_file):
    # Read the CSV file
    data = pd.read_csv(csv_file, header=None, names=["Returns", "Epsilon"])

    # Create the plot
    plt.figure(figsize=(10, 6))
    plt.plot(data["Returns"], label="Average Returns")
    plt.plot(data["Epsilon"], label="Epsilon")
    plt.xlabel("Epochs")
    plt.ylabel("Value")
    plt.title("Training Progress")
    plt.legend()
    plt.grid(True)

    # Save the plot to a file
    plt.savefig(output_file)
    plt.close()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python3 plot.py <input_csv> <output_png>")
        sys.exit(1)

    csv_file = sys.argv[1]
    output_file = sys.argv[2]
    generate_plot(csv_file, output_file)