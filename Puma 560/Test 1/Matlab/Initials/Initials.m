% Load in trejectory

% Initials AS
% path = [0 0 1;0 0 0;0 2 0;1 2 0;1 0 0;1 0 1;0 1 1;0 1 0;1 1 0;1 1 1;1.5 0.5 1;1.5 0.5 0;1.5 0 0;2.5 0 0;2.5 0.5 0;1.5 1.5 0;1.5 2 0;2.5 2 0;2.5 1.5 0;2.5 1.5 1;]

% Name ALEX
path = [0 0 1;0 0 0;0 2 0;1 2 0;1 0 0;1 0 1;0 1 1;0 1 0;1 1 0;1 1 1;1.5 2 1;1.5 2 0;1.5 0 0;2.5 0 0; 2.5 0 1;4 2 1;4 2 0;3 2 0;3 0 0;4 0 0;4 0 1;3 1 1;3 1 0;4 1 0;4 1 1;4.5 2 1;4.5 2 0;5.5 0 0;5.5 0 1;5.5 2 1;5.5 2 0;4.5 0 0;4.5 0 1;]

% TRAJ = MSTRAJ(P, QDMAX, TSEG, Q0, DT, TACC, OPTIONS)
% trajectory = mstraj(path,[speedx speedy speedz],[],[initx inity initz],sampleperiod,acceltime)
point = mstraj(path,[.5 .5 .5],[],[0 0 1],0.02,0.2) % Create a trajectory of points

% - P (MxN) is a matrix of via points, 1 row per via point, one column 
%   per axis.  The last via point is the destination.
% - QDMAX (1xN) are axis speed limits which cannot be exceeded,
% - TSEG (1xM) are the durations for each of the K segments
% - Q0 (1xN) are the initial axis coordinates
% - DT is the time step
% - TACC (1x1) this acceleration time is applied to all segment transitions
% - TACC (1xM) acceleration time for each segment, TACC(i) is the acceleration 
%   time for the transition from segment i to segment i+1.  TACC(1) is also 
%   the acceleration time at the start of segment 1.

% Plot 3D trajectory
% Part B: A plot of the end effector trajectory in 3 Dimensions (X,Y,Z)
subplot(3,1,1), plot3(point(:,1),point(:,2),point(:,3))
xlabel('x'); ylabel('y'); zlabel('z');title('trajectory')

% Cartesian Space Method
traj = transl(0.1*point) % Translation of 0.1 Pose
Transform = homtrans(transl(0.4,0,0),traj) % HomoTrans Pose (0.4,0,0) (pose1,pose2,time) of the trajectory

% T = transl(0.1*p) % Scale to 10% size
% T_Final = homtrans(transl(0.4,0,0),T)

% Using Peter Corke's robotics toolbox, as the avaliable mdl_puma560 is already
% done for us / traj = ctraj(pose1,pose2,#steps)
mdl_puma560
p560.tool = trotx(pi)
angle = p560.ikine6s(Transform) % 50 x poses consisting of 6 joint angles, inverse kinematic of the transform
time = [0:0.05:6]' % 2 Seconds, 50 ms period
% The ? causes the vector to be transposed

% total_time = timesteps * step time
% total_time = numrows(p)*0.02 
% Total Time = 32 points * 0.02 = 0.64

% p560.tool = trotx(pi)
% q = p560.ikine6s(T_Final)
% p560.plot(q)

% Part C: A plot showing each of the joint angles with respect to time
subplot(3,1,2), p560.plot(time,angle)
xlabel('time(s)'); ylabel('joint angle (rad)');title('joint angle / time')
legend('J1','J2','J3','J4','J5','J6')

% Part D: A plot showing XYZRPY magnitudes with respect to time.
subplot(3,1,3), plot(time,tr2rpy(Transform)) % Orientation component
xlabel('time (s)'); ylabel('orientation (rad)');title('xyz, roll, pitch, yaw')
legend('Roll','Pitch','Yaw')

% A video file that shows the robot simulation
fig = figure('visible','on');

fps = 25;
n_samples = 5 * fps;

mov.frames = getframe(fig);

filename = 'Demo';
% writerObj.CompressionRatio = 3;
writerObj = VideoWriter('Demo', 'MPEG-4', 'fps', 3);


open(writerObj);
for i = 1:2000, % 50*0.02 * secconds (0.64)
    p560.plot(gait(angle,i,0,0));
    drawnow;
    frames(i) = getframe(fig);
    writeVideo(writerObj,frames);
end
close(writerObj);