# Advent of Code solution files

Merry Christmas and Happy New Year!

I got 46 stars in Advent of Code 2023.

## Structure

Here is my solutions for Advent of Code problems. Here is only my final files
so solutions for first star are not present in every file.

I created template for problems that I use with command:
```
./start <day>
```

I execute with example input using
```
make
```
and with puzzle input using
```
make test
```

Most solutions have following format:

```
void read() {
  // ...
}

void first() {
  // ...
}

void second() {
  // ...
}

int main() {
  read();
  second();
}
```
