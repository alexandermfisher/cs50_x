from sys import argv, exit
from csv import DictReader
from re import findall

def main():
    # ensure correct usage:
    if (len(argv) != 3):
        exit('Usage: python dna.py FILENAME FILENAME')

    # read in databases containing  STR counts for individuals.
    database = []
    with open(argv[1]) as file:
        reader = DictReader(file)
        fields = reader.fieldnames[1:]
        for row in reader:
            for field in fields:
                try:
                    row[field] = int(row[field])
                except ValueError:
                    pass
            database.append(row)

    # read in individuals_sequence for testing against database.
    with open(argv[2]) as file:
        sequence = file.read().strip()

    # find max number STR patterns appear and store in sequence_dict:
    sequence_dict = {}
    for field in fields:
        try:
            sequence_dict[field] = int(len(max(findall(f'(({field})+)', sequence), key = lambda x: len(x[0]))[0]) / len(field))
        except ValueError:
            sequence_dict[field] = 0

    # find a match if at all possible and print to console
    sequence_counts = list(sequence_dict.values())
    for individual in database:
        individual_counts = list(individual.values())[1:]
        if individual_counts == sequence_counts:
            print(individual['name'])
            return

    print('No match')

    return



if __name__ == '__main__':
    main()
