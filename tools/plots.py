import seaborn as sns # type: ignore
import matplotlib.pyplot as plt # type: ignore

with open('build/norm_dist_random_numbers.txt', 'r') as file:
    numbers = [int(line.strip()) for line in file]

plt.figure(figsize=(12, 8))  # Enlarge figure dimensions
sns.set_theme(font_scale=1.5)
sns.histplot(numbers, kde=True)
plt.title('Distribution of random 20 000 numbers generated using get_normal(10000.0, 150.0)')
plt.xlabel('Value')
plt.ylabel('Count')
plt.tight_layout(pad=1.5)
plt.savefig('get_normal_plot.png')


with open('build/dist_random_numbers.txt', 'r') as file:
    numbers = [int(line.strip()) for line in file]

plt.figure(figsize=(12, 8))
sns.set_theme(font_scale=1.5)
sns.histplot(numbers)
plt.title('Distribution of random 20 000 numbers generated using get_next(1000, 1500)')
plt.xlabel('Value')
plt.ylabel('Count')
plt.tight_layout(pad=1.5)
plt.savefig('get_next_plot.png')