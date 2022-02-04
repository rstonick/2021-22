#!/usr/bin/env python2

import roslib
import rospy
import smach
import smach_ros
import time

from rover.msg import cameras2smach
from rover.msg import controls2smach
from rover.msg import lidar2smach
from rover.msg import sensors2smach

from subscriber import *


class Post_Code(smach.State):
	def __init__(self):
		print("Compiling Post_Code State")
		smach.State.__init__(self, outcomes = ['Post Completed'])
            	#Subscribe to all nodes that publish to smach
		self.sensors_sub = Subscribe_to('sensors2smach')
		self.lidar_sub = Subscribe_to('lidar2smach')
		self.cameras_sub = Subscribe_to('cameras2smach')
		self.controls_sub = Subscribe_to('controls2smach')
		self.counter = 0
		time.sleep(2)
		

	def execute(self, userdata):
		i = 0
		while ((i == 0) or (i>1) or (i<1)):
			print("")
			print("please input a number")
			print("1:Post Complete")
			i = input()
			if(i == 1):
				return 'Post Completed'

		

def code():
	rospy.init_node('sm')
	main = smach.StateMachine(outcomes = ['Mission_Complete'])
	with main:
		smach.StateMachine.add('Post_Code', Post_Code(), transitions = {'Post Completed':'Mission_Complete'})

	sis = smach_ros.IntrospectionServer('server' , main , '/tester')
	sis.start()
	outcome = main.execute()
	sis.stop()
	rospy.spin()

if __name__ == '__main__':
	code()
