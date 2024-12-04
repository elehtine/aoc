from sys import stdin

star = True

def main():
    numbers = "0123456789"

    transitions = dict()
    transitions["start"] = { "m": "m", "d": "d" }
    transitions["m"] = { "u": "u" }
    transitions["u"] = { "l": "l" }
    transitions["l"] = { "(": "first" }
    transitions["first"] = { ",": "second" }
    transitions["second"] = { ")": "start" }
    transitions["d"] = { "o": "o" }
    transitions["o"] = { "(": "do", "n": "n" }
    transitions["do"] = { ")": "start" }
    transitions["n"] = { "'": "'" }
    transitions["'"] = { "t": "t" }
    transitions["t"] = { "(": "dont" }
    transitions["dont"] = { ")": "start" }

    state = "start"
    first = 0
    second = 0
    result = 0
    dont = False

    for line in stdin:
        for character in line:
            if state == "start":
                first = 0
                second = 0

            if state == "first" and character in numbers:
                first *= 10
                first += ord(character) - ord('0')
                continue

            if state == "second" and character in numbers:
                second *= 10
                second += ord(character) - ord('0')
                continue

            if state == "second" and character == ")":
                if not dont or star:
                    result += first * second

            if state == "dont" and character == ")":
                dont = True

            if state == "do" and character == ")":
                dont = False

            state = transitions[state].get(character, "start")

    print(result)

if __name__ == "__main__":
    main()
