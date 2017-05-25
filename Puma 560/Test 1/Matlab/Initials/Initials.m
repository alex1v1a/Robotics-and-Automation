% Load in trejectory
path = [0 0 1;0 0 0;0 2 0;1 2 0;1 0 0;1 0 1;0 1 1;0 1 0;1 1 0;1 1 1;1.5 2 1;1.5 2 0;1.5 0 0;2.5 0 0; 2.5 0 1;4 2 1;4 2 0;3 2 0;3 0 0;4 0 0;4 0 1;3 1 1;3 1 0;4 1 0;4 1 1;4.5 2 1;4.5 2 0;5.5 0 0;5.5 0 1;5.5 2 1;5.5 2 0;4.5 0 0;4.5 0 1;]
point = mstraj(path,[.5 .5 .5],[],[0 0 1],0.02,0.2)

% Plot 3D trajectory
% Part B: A plot of the end effector trajectory in 3 Dimensions (X,Y,Z)
subplot(3,1,1), plot3(point(:,1),point(:,2),point(:,3))
xlabel('x'); ylabel('y'); zlabel('z');title('trajectory')

T = transl(0.1*point)
Transform = homtrans(transl(0.4,0,0),T)

% Using Peter Corke's robotics toolbox, as the avaliable mdl_puma560 is already
% done for us
mdl_puma560
p560.tool = trotx(pi)
angle = p560.ikine6s(Transform)
time = [0:0.001:2.409]'

% % Part C: A plot showing each of the joint angles with respect to time
% subplot(3,1,2), plot(time,angle)
% xlabel('time(s)'); ylabel('joint angle (rad)');title('joint angle / time')
% legend('J1','J2','J3','J4','J5','J6')
% 
% % Part D: A plot showing XYZRPY magnitudes with respect to time.
% subplot(3,1,3), plot(time,tr2rpy(Transform))
% xlabel('time (s)'); ylabel('orientation (rad)');title('xyz, roll, pitch, yaw')
% legend('Roll','Pitch','Yaw')

% % A video file that shows the robot simulation
% video = figure('visible','on');
% mov.frame = getframe(video);
% 
% filename = 'Demo';
% % writerObj.CompressionRatio = 3;
% writerObj = VideoWriter('Demo', 'MPEG-4', 'fps', 3);
% 
% 
% open(writerObj);
% for i = 1:2000,
%     p560.plot(gait(angle,i,0,0))
%     drawnow;
%     frame(i) = getframe(video);
%     writeVideo(writerObj,frame);
% end
% close(writerObj);