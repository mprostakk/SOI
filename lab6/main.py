from fs import create_virtual_disk, read_virtual_disk


def main():
    # create_virtual_disk()

    superblock, inodes, blocks = read_virtual_disk()
    print(superblock)
    print(inodes)
    print(blocks)

    print(1)


if __name__ == "__main__":
    main()
