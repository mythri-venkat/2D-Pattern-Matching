from os import *
import random

def main():
    n = 20
    m = 4
    for i in range(0,m):
        for j in range(0,m):
            pat[i][j] = chr(random.randint(0,25)+ord('a'))

    for i in range(0,n):
        for j in range(0,n):
            text[i][j] = chr(random.randint(0,25)+ord('a'))

    matches = []

