#!/usr/bin/python
import os
import sys
import cv2
import glob
import numpy as np
import threading
import pdb

Len = 75
Stride = 30

def is_pos(event,start,end):
    for i in xrange(event.shape[0]):
        x = max(start,event[i][0])
        y = min(end,event[i][1])
        if x >= y:
            continue
        if (y - x)/float(end-start) > 0.5:
            return True
    return False

def slice_video(video):
    video = video.split('/',1)[1]
    event_span = 'personruns_span/' + video.split('.',1)[0] + '.eventSpan'
    event = np.loadtxt(event_span,usecols=(2,3),delimiter='\t',dtype=np.int32)
    capture = cv2.VideoCapture('video/' + video)
    frame_sum = capture.get(cv2.cv.CV_CAP_PROP_FRAME_COUNT)
    line = video.split('.',1)[0]
    start = 0
    end = start + Len
    while end <= frame_sum-Len:
        capture.set(cv2.cv.CV_CAP_PROP_POS_FRAMES,start)
        fid_num = 0
        if is_pos(event,start,end):
            path = 'slice_video/pos/'+line
        else:
            path = 'slice_video/neg/'+line
        if not os.path.exists(path):
            os.mkdir(path)
        path = path + '/' + line + '_{}_{}.avi'.format(start,end-1)
        fps = capture.get(cv2.cv.CV_CAP_PROP_FPS)
        size = (int(capture.get(cv2.cv.CV_CAP_PROP_FRAME_WIDTH)),
                int(capture.get(cv2.cv.CV_CAP_PROP_FRAME_HEIGHT)))
        writer = cv2.VideoWriter(path,cv2.cv.CV_FOURCC('M', 'J', 'P', 'G'),25,size)
        while fid_num < Len:
            ret,frame = capture.read()
            if ret == False:
                break
            writer.write(frame)
            fid_num += 1
        start += Stride
        end += Stride

class myThread (threading.Thread):
    def __init__(self,video):
        threading.Thread.__init__(self)
        self.video = video
    def run(self):
        print "Starting Slice" + self.video
        slice_video(self.video)
        print "Complete Slice " + self.video

def main():
    videos = glob.glob('video/*')
    my_thread= []
    my_thread = [myThread(video) for video in videos]
    for t in my_thread:
        t.start()
    for t in my_thread:
        t.join()

if __name__ == "__main__":
    main()
