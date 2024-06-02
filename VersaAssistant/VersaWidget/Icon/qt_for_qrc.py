import subprocess, os, argparse

# def run_cmd(cmd,SourceName = 'MingCute'):
#     print('cmd={}'.format(cmd))
#     subprocess.Popen(str(cmd), stdout=subprocess.PIPE, stdin=subprocess.PIPE,stderr=subprocess.PIPE, creationflags=0x08)
#     print(SourceName)


def write_qrc(SourceName):
    # 1.批量重命名
    # SourceName = 'MingCute'
    SourcePath = ".\\" + SourceName
    # qss_path='.\\qss\\'
    ListFolder = os.listdir(SourcePath)
    images_new = []
    renameTotal = 1

    for FolderName in ListFolder:
        ListFile = os.listdir(SourcePath + "\\" + FolderName)
        for File in ListFile:
            addCount = 1
            item_new = (
                str(File)
                .replace(" ", "_")
                .replace("\n", "_")
                .replace("%", "_")
                .replace("￥", "_")
                .replace("副本", "_")
                .replace("&", "_")
                .replace("-", "_")
                .replace(".", "_")
                .replace("#", "_")
                .replace("@", "_")
                .replace("(", "_")
                .replace(")", "_")
                .replace("（", "_")
                .replace("）", "_")
                .replace("\t", "_")
                .replace("_png", ".png")
                .replace("_jfif", ".jfif")
                .replace("_zip", ".zip")
                .replace("_ico", ".ico")
                .replace("_svg", ".svg")
                .replace("_rar", ".rar")
                .replace("___", "_")
                .replace("__", "_")
            )

            filename = os.path.splitext(File)[0]  # 文件名
            filetype = os.path.splitext(File)[1]  # 文件名
            while item_new in images_new:
                item_new = filename + "_" + str(addCount) + filetype
                addCount += 1

            images_new.append(item_new)
            # print('重命名第 {} 张图片， {} => {}'.format(renameTotal,File,item_new))
            os.rename(
                SourcePath + "\\" + FolderName + "\\" + File,
                SourcePath + "\\" + FolderName + "\\" + item_new,
            )  # 重命名
            renameTotal += 1
    print("重命名 {} 张图片".format(renameTotal))

    """
    2.生成qrc文件，包含需要操作的图片资源的目录
    """
    ListFolder = os.listdir(SourcePath)  # 填入相对路径目录名
    # qss = os.listdir(qss_path)           #填入相对路径目录名
    pFile = open(SourceName + ".qrc", "w+")
    pFile.write('<!DOCTYPE RCC>\n<RCC version="1.0">\n\t<qresource prefix="/">\n')
    for FolderName in ListFolder:
        ListFile = os.listdir(SourcePath + "\\" + FolderName)
        for File in ListFile:
            # print(u'<file>' + SourceName +'/' + FolderName + '/' + File + '</file>\n')
            pFile.write(
                "\t\t<file>" + SourceName + "/" + FolderName + "/" + File + "</file>\n"
            )
    pFile.write("</qresource>\n</RCC>")
    pFile.close()


parser = argparse.ArgumentParser(description="Test for argparse")
parser.add_argument("--source", "-s", help="source 资源文件名称")
args = parser.parse_args()

if __name__ == "__main__":
    try:
        write_qrc(args.source)
    except Exception as e:
        print(e)


# for item in qss:
#     f.write(u'<file alias="qss/' + item + '">qss/' + item + '</file>\n')

# 3.生成qrc文件
# 获取程序当前路径
# program_current_path=os.path.abspath('.')
# print('program_current_path={}'.format(program_current_path))

# make_qrc='pyrcc5 -o .\images_rc.py .\images.qrc'
# run_cmd(make_qrc)
