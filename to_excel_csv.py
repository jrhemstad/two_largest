# Copy and paste your gbench results into a file 
# name gbench_results and run this script

with open('gbench_results') as f:
    for i, line in enumerate(f):
        [label, time] = line[9:].split()[:2]
        if i % 6 == 0:
            if i != 0: print()
            print(label.split('/')[0],end=',')
        print(time,end=',')
print()
