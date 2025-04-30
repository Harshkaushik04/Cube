import sys
import pandas as pd
import matplotlib.pyplot as plt

def generate_plot(csv_file, output_file):
    # Read the CSV file
    data = pd.read_csv(csv_file, header=None, names=["Returns", "Epsilon","EpsilonStage2","lowRewardsAverage","mediumRewardsAverage","highRewardsAverage","veryHighRewardsAverage"])

    # Create the plot
    plt.figure(figsize=(10, 6))
    plt.plot(data["Returns"], label="Average Returns")
    plt.plot(data["Epsilon"], label="Epsilon")
    plt.plot(data["EpsilonStage2"], label="Epsilon Stage2")
    plt.plot(data["lowRewardsAverage"], label="not completed stage 1")
    plt.plot(data["mediumRewardsAverage"], label="completed stage 1 only")
    plt.plot(data["highRewardsAverage"], label="completed till stage 2")
    plt.plot(data["veryHighRewardsAverage"], label="completed stage 3 or higher")
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