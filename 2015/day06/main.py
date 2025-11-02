


from re import findall

def day6(input):
    actions = {
        'toggle': lambda x: x + 2,
        'turn on': lambda x: x + 1,
        'turn off': lambda x: x - 1 if x > 0 else 0
    }
    # actions = {
    #     'toggle': lambda x: not x,
    #     'turn on': lambda x: 1,
    #     'turn off': lambda x: 0
    # }
    lights = [[0 for i in range(1000)] for j in range(1000)]
    instructions = findall("(toggle|turn on|turn off)\s(\d*),(\d*)\sthrough\s(\d*),(\d*)", input)
    for action, x1, y1, x2, y2 in instructions:
        coords = [(x, y) for x in range(int(x1), int(x2) + 1) for y in range(int(y1), int(y2) + 1)]
        for x, y in coords:
            lights[x][y] = actions[action](lights[x][y])
    flattened = [val for sublist in lights for val in sublist]
    return sum(flattened)

with open('day06/input.txt', 'r') as input:
    print(day6(input.read()))