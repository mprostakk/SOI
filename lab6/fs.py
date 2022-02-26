import fs_parameters
from block import Block
from inode import INode
from superblock import SuperBlock


def create_filesystem(superblock: SuperBlock, inodes: list[INode], blocks: list[Block]) -> bytearray:
    filesystem = bytes(superblock)
    for inode in inodes:
        b = bytes(inode)
        filesystem += b

    for block in blocks:
        b = bytes(block)
        filesystem += b

    return bytearray(filesystem)


def save_file_system_to_file(filesystem: bytearray, filename: str = "filesystem"):
    with open(filename, "wb") as file:
        file.write(filesystem)


def read_filesystem_from_file(filename: str = "filesystem") -> bytearray:
    with open(filename, "rb") as file:
        filesystem = file.read()
    return bytearray(filesystem)


def create_filesystem_from_bytes(filesystem: bytearray):
    index = 0
    superblock = SuperBlock.create_from_bytes(filesystem[index:index+SuperBlock.number_of_bytes()])
    index += SuperBlock.number_of_bytes()

    inodes = []
    for i in range(fs_parameters.NUMBER_OF_INODES):
        n = INode.create_from_bytes(filesystem[index:index+INode.number_of_bytes()])
        inodes.append(n)
        index += INode.number_of_bytes()

    blocks = []
    for i in range(fs_parameters.NUMBER_OF_BLOCKS):
        b = Block.create_from_bytes(filesystem[index:index+Block.number_of_bytes()])
        blocks.append(b)
        index += Block.number_of_bytes()

    return superblock, inodes, blocks


def create_virtual_disk():
    superblock = SuperBlock(magic_number=777, number_of_inodes_blocks=fs_parameters.NUMBER_OF_INODES, number_of_inodes=0,
                            number_of_blocks=fs_parameters.NUMBER_OF_INODES * 10)
    inodes: list[INode] = [INode(number=x, type=2, direct_pointers=[x for x in range(10)], indirect_pointer=9) for x in range(
        fs_parameters.NUMBER_OF_INODES)]
    blocks: list[Block] = [Block(data=b"1234567890", next_block=0) for x in range(fs_parameters.NUMBER_OF_BLOCKS)]

    filesystem = create_filesystem(superblock, inodes, blocks)
    save_file_system_to_file(filesystem)


def read_virtual_disk():
    filesystem = read_filesystem_from_file()
    return create_filesystem_from_bytes(filesystem)
