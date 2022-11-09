# MyOS

This is the synchronous code of my [Operating System Implementation Journal](https://blog.linloir.cn/2022/07/15/os-journal-vol-1/).

Every commit corresponds to a chapter of my journal

## Run

```bash
clone https://github.com/Linloir/MyOS.git
cd MyOS/build
sudo make clean build && make run
```

**Note**: In order to place kernel file inside the file system, losetup and mount command is used in makefile, which required root privilege. Therefore sudo is needed for make build

## Debug

```bash
make debug
```

## References

Below are articles that had helped me a lot
