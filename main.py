# This Python file uses the following encoding: utf-8
import os
import sys
from pathlib import Path
import cv2

import numpy as np

from PySide6.QtCore import QObject, Slot
from PySide6.QtQml import QQmlApplicationEngine
from PySide6.QtWidgets import QFileDialog, QApplication

base_dir = os.path.dirname(os.path.realpath(sys.argv[0]))
if os.path.exists(base_dir + "/dll"):
    os.add_dll_directory(base_dir + "/dll")

# import QtQuick
sys.path.append("Cpp_Core/x64/Release")
import SDF_Cpp



class SDFLib:
    def __init__(self, parent=None):
        super().__init__(parent)

    @staticmethod
    def SDF_Generate(folder_path):
        output_folder = ""
        for filename in os.listdir(folder_path):
            file_path = os.path.join(folder_path, filename)
            # 检查文件是否是图像文件
            if os.path.isfile(file_path) and filename.lower().endswith(('.jpg', '.jpeg', '.png', '.gif', '.bmp')):
                image = cv2.imread(file_path)
                output_folder = SDF_Cpp.GenerateSDF(folder_path, filename)
        return output_folder

    @staticmethod
    def lerp_SDF(folder_path):
        sdf_folder = SDFLib.SDF_Generate(folder_path)
        if sdf_folder.strip() != "":
            print("SDF生成完成")
            SDF_Cpp.SDFLerp(sdf_folder)
            print("SDF插值完成")

    @staticmethod
    def make_Atlas(folder_path, row, col, resolution_x, resolution_y, is_topdown_one_texture):
        images = np.empty((row, col), dtype=object)
        res_x = resolution_x / row
        for filename in os.listdir(folder_path):
            if filename.lower().endswith(('.jpg', '.jpeg', '.png', '.gif', '.bmp')):
                parts = os.path.splitext(filename)[0].split('_')
                if len(parts) >= 2:
                    i = parts[-2]
                    j = parts[-1]
                    if i.isdigit() and j.isdigit():
                        filepath = os.path.join(folder_path, filename)
                        img = cv2.imread(filepath)
                        images[int(i)-1][int(j)-1] = img
                        # print(f'{i},{j}')

        if is_topdown_one_texture:
            for i in range(1, col):
                images[0][i] = images[0][0]
                images[row - 1][i] = images[row - 1][0]

        combine = None
        for i in range(row):
            row_images = images[i * col: (i + 1) * col]
            row_concatenated = cv2.hconcat(images[i])
            if combine is None:
                combine = row_concatenated
            else:
                combine = cv2.vconcat([combine, row_concatenated])

        print(combine.shape)
        result = cv2.resize(combine, (resolution_x, resolution_y))
        is_generated = cv2.imwrite(folder_path + "/SDF_Atlas.png", result)
        if is_generated:
            print(f"图集生成成功，路径：{folder_path}/SDF_Atlas.png")


class FuncForQml(QObject):

    def __init__(self, parent=None):
        super().__init__(parent)

    @Slot(result=str)
    def selectPath(self):
        folder_path = QFileDialog.getExistingDirectory(None, 'Open Folder', "./")
        if folder_path:
            print(f'Selected folder path: {folder_path}')
        return folder_path

    @Slot(str)
    def generateSDF(self, path):
        SDFLib.lerp_SDF(path)

    @Slot(str, int, int, int, int, bool)
    def generateAtlas(self, path, row, col, resolution_x, resolution_y, is_topdown_one_texture):
        SDFLib.make_Atlas(path, row, col, resolution_x, resolution_y, is_topdown_one_texture)


if __name__ == "__main__":
    app = QApplication(sys.argv)
    engine = QQmlApplicationEngine()

    pyFunc = FuncForQml()
    engine.rootContext().setContextProperty("pyFunc", pyFunc)

    qml_file = Path(__file__).resolve().parent / "main.qml"
    engine.load(qml_file)

    if not engine.rootObjects():
        sys.exit(-1)
    sys.exit(app.exec())
