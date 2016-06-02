import glob

list = glob.glob('/home/zyy/workspace/wangml/improved_trajectory_release/slice_video/*/*/*')

with open('list.txt','w') as f:
	for li in list:
		f.write('{}\n'.format(li))
