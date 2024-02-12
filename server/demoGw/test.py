import time 

# print('why no working python')
def main():
    print('now start test.py')
    mainCount = 0
    while True:
        # try:
        mainCount += 1
        time.sleep(1)
        print('main: {}'.format(mainCount))


if __name__ == '__main__':
    main()
