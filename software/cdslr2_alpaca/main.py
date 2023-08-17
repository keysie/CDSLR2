import os
import platform
import rawpy
import matplotlib.pyplot as plt


class MTP_Camera:
    def __init__(self, mount_point: str, sub_folder: str) -> None:
        self.mountpt = mount_point
        self.imgpath = os.path.join(mount_point, sub_folder)
        if not os.path.exists(self.imgpath):
            FileNotFoundError()

    def get_latest_picture(self):
        latest_img_path = os.path.join(self.imgpath, os.listdir(self.imgpath)[-1])
        if not os.path.isfile(latest_img_path):
            FileNotFoundError()
        with rawpy.imread(latest_img_path) as raw:
            #rgb = raw.postprocess()
            return raw.raw_image.copy()

    def unmount_fs(self):
        if platform.system() == 'Windows':
            os.system(
                'powershell $driveEject = New-Object -comObject Shell.Application;' +
                '$driveEject.Namespace(17).ParseName("""' + self.mountpt + '""").InvokeVerb("""Eject""")'
                )
        else:
            #TODO: Implement code to handle unmounting in other OSes
            NotImplementedError('Unmounting FS currently only work on Windows. Sorry.')


my_camera = MTP_Camera(mount_point='G:', sub_folder='/DCIM/100MSDCF')
try:
    img = my_camera.get_latest_picture()
    plot = plt.imread(img)
    plt.show()
except FileNotFoundError as ex:
    print(ex)
