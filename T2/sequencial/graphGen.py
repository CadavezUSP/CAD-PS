import random

def genGraph(nodes):
    graph = {}
    for i in range(nodes):
        for j in range(nodes):
            if i != j:
                peso = random.randint(1, 10)
                graph[(i, j)] = peso
    return graph


def main():
    graph = genGraph(10)
    for i in graph:
        print(i[0], i[1], graph[i])

if __name__ == "__main__":
    main()