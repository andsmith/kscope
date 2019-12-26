import cv2
import numpy as np

def extend_3d(p_2d):
    v = np.zeros(3).reshape(3)
    v[:2] = p_2d
    return v


class Mirror(object):
    def __init__(self, point, norm):
        self._p = point
        self._n = norm

    def __str__(self):
        return "\tpoint:  %s\n\tvect:  %s" % (
            ", ".join(["%.3f"%(p,) for p in self._p]), 
            ", ".join(["%.3f"%(p,) for p in self._n]))

    @staticmethod
    def from_points(pa, pb):
        import ipdb; ipdb.set_trace()
        p0 = extend_3d(pa)
        p1 = extend_3d(pb)
        parallel = p1 - p0
        parallel = parallel/ np.linalg.norm(parallel)
        point = (p0+p1)/2.0

        to_zero = np.zeros(3) - point

        perp = to_zero - parallel * np.dot(parallel, to_zero)
        perp = perp/np.linalg.norm(perp)
        return Mirror(point, perp)
        
    def reflect_rays(self, ray_starts, ray_dirs):
        pass

        
class KScope(object):
    def __init__(self, src_image_file, out_size):
        self._src_file = src_image_file;
        self._base_img = cv2.imread(src_image_file)
        self._out_size = out_size
        self._mirrors = []
        self._height_cm = 20.0;
        self._angular = np.radians(35.0)


    def set_isolateral(self, height, base):
        p0 = np.array([0.0, 0.0]);
        p1 = np.array([-base/2.0, -height]);
        p2 = np.array([base/2.0, -height]);
        self._mirrors.append(Mirror.from_points(p0, p1))
        self._mirrors.append(Mirror.from_points(p1, p2))
        self._mirrors.append(Mirror.from_points(p2, p0))

    def print_mirrors(self):
        for i, m in enumerate(self._mirrors):
            print("Mirror %i:"% (i,))
            print(m)

if __name__=="__main__":
    k = KScope('forest.jpg', 512)
    k.set_isolateral(3.0, 2.0)
    k.print_mirrors()
