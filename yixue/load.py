import os
import numpy as np
import json

# label name is ['cost_time', 'course_id', 'difficulty', 'is_right', 'question_end_time', 
# 		'question_end_time_delta', 'question_id', 'question_start_time', 'question_start_time_delta', 
# 		'right_answer', 'school_id', 'section_id', 'subject', 'topic_id', 'user_answer', 'user_id', 'filename']

data=np.load("webcam_feature.npy")
eye=np.load("webcam_eye_feature.npy")
label=np.load("webcam_label.npy")
filename=np.load("webcam_filename.npy")
labelname=np.load("webcam_labelname.npy")
pos=np.load("webcam_pos.npy")
confidence=np.load("webcam_confidence.npy")




