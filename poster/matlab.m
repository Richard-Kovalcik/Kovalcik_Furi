%%
%Use PseudoCode for poster. 
%Z FORCE

% a used to reference what goes on in each column
% this will be to plot the positive z force
a = readtable("zforce.csv")

%csvread turns nans to 0s
t = csvread("zforce.csv",1,0);

%code for processing z force
preprocessed_zforce = [t(:,27)];
for i= 2:1:11135 
    
    if (preprocessed_zforce(i,1) == 0) 
       
        preprocessed_zforce(i,1) = preprocessed_zforce(i-1,1);

    end
    
end

%code for processing angular velocities force
angular_velocities = [t(:,2:7)]
for k= 1:1:6    
    for i= 2:1:11135 
        
        if (angular_velocities(i,k) == 0) 
           
            angular_velocities(i,k) = angular_velocities(i-1,k);
    
        end
        
    end
end

%code for getting the average force over the interval
zforce_avg = mean( preprocessed_zforce(1500:10000,1))

%code for getting the rpm values of each motor
figure;

%code for plotting
subplot(2,2,1);
plot(preprocessed_zforce)
ylabel ('Force Along z Axis (Newtons)','FontWeight', 'bold')
xlabel ('Time (Milliseconds)','FontWeight', 'bold')
text(5000,0,'Zforce_a_v_g = -11.6308','FontWeight','bold')

subplot(2,2,3);
plot(angular_velocities)
ylabel ('Radians Per Second','FontWeight', 'bold')
xlabel ('Time (Milliseconds)','FontWeight', 'bold')
text(5000,400,'Final Angular Velocity_1 = 597.07','FontWeight','bold')
text(5000,350,'Final Angular Velocity_2 = 620.91','FontWeight','bold')
text(5000,300,'Final Angular Velocity_3 = 599.18','FontWeight','bold')
text(5000,250,'Final Angular Velocity_4 = 623.35','FontWeight','bold')
text(5000,200,'Final Angular Velocity_5 = 605.43','FontWeight','bold')
text(5000,150,'Final Angular Velocity_6 = 604.32','FontWeight','bold')
%%
% POS Z TORQUE

a1 = readtable("posztorque.csv")

%csvread turns nans to 0s
t1 = csvread("posztorque.csv",1,0);

%code for processing pos z torque
z_torque = [t1(:,30)];
for i= 2:1:11390 
    
    if (z_torque(i,1) == 0) 
       
       z_torque(i,1) = z_torque(i-1,1);

    end
    
end


%code for processing angular velocities force
angular_velocities1 = [t1(:,2:7)]
for k= 1:1:6    
    for i= 2:1:11390 
        
        if (angular_velocities1(i,k) == 0) 
           
            angular_velocities1(i,k) = angular_velocities1(i-1,k);
    
        end
        
    end
end

ztorque_avg = mean( z_torque(2000:10000,1))

%code for plotting
subplot(2,2,2);
plot(z_torque)
ylabel ('Torque About Z Axis (N/M)','FontWeight', 'bold')
xlabel ('Time (Milliseconds)','FontWeight', 'bold')
text(5000,-0.2,'Positive Ztorque_a_v_g = 0.0452','FontWeight','bold')

subplot(2,2,4);
plot(angular_velocities1)
ylabel ('Radians Per Second','FontWeight', 'bold')
xlabel ('Time In Milliseconds','FontWeight', 'bold')
text(5000,400,'Final Angular Velocity_1 = 0.00','FontWeight','bold')
text(5000,350,'Final Angular Velocity_2 = 626.21','FontWeight','bold')
text(5000,300,'Final Angular Velocity_3 = 0.00','FontWeight','bold')
text(5000,250,'Final Angular Velocity_4 = 636.23','FontWeight','bold')
text(5000,200,'Final Angular Velocity_5 = 616.19','FontWeight','bold')
text(5000,150,'Final Angular Velocity_6 = 0.00','FontWeight','bold')
%%
%POS X Torque
% Will only use positive values on graph to not take up entire poster
% create a new figure for the next 4 graphs
figure

a2 = readtable("xtorque.csv")

%csvread turns nans to 0s
t2 = csvread("xtorque.csv",1,0);

%code for processing pos z torque
x_torque = [t2(:,28)];
for i= 2:1:10560
    
    if (x_torque(i,1) == 0) 
       
       x_torque(i,1) = x_torque(i-1,1);

    end
    
end


%code for processing angular velocities force
angular_velocities2 = [t2(:,2:7)]
for k= 1:1:6    
    for i= 2:1:10560 
        
        if (angular_velocities2(i,k) == 0) 
           
            angular_velocities2(i,k) = angular_velocities2(i-1,k);
    
        end
        
    end
end

xtorque_avg = mean( x_torque(1000:9000,1))

subplot(2,2,1);
plot(x_torque)
ylabel ('Torque About X Axis (N/M)','FontWeight', 'bold')
xlabel ('Time (Milliseconds)','FontWeight', 'bold')
text(5000,1,'Positive Xtorque_a_v_g = 2.05','FontWeight','bold')

subplot(2,2,3);
plot(angular_velocities2)
ylabel ('Radians Per Second','FontWeight', 'bold')
xlabel ('Time (Milliseconds)','FontWeight', 'bold')
text(5000,400,'Final Angular Velocity_1 = 0.00','FontWeight','bold')
text(5000,350,'Final Angular Velocity_2 = 628.75','FontWeight','bold')
text(5000,300,'Final Angular Velocity_3 = 617.68','FontWeight','bold')
text(5000,250,'Final Angular Velocity_4 = 0.00','FontWeight','bold')
text(5000,200,'Final Angular Velocity_5 = 0.00','FontWeight','bold')
text(5000,150,'Final Angular Velocity_6 = 620.31','FontWeight','bold')
%%
%POS Y TORQUE

a3 = readtable("ytorque.csv")

%csvread turns nans to 0s
t3 = csvread("ytorque.csv",1,0);

%code for processing pos z torque
y_torque = [t3(:,29)];
for i= 2:1:10343
    
    if (y_torque(i,1) == 0) 
      
       y_torque(i,1) = y_torque(i-1,1);

    end
    
end


%code for processing angular velocities force
angular_velocities3 = [t3(:,2:7)]
for k= 1:1:6    
    for i= 2:1:10343
        
        if (angular_velocities3(i,k) == 0) 
           
            angular_velocities3(i,k) = angular_velocities3(i-1,k);
    
        end
        
    end
end

ytorque_avg = mean( y_torque(1000:9000,1))

%code for plotting
subplot(2,2,2);
plot(y_torque)
ylabel ('Torque About Y Axis (N/M)','FontWeight', 'bold')
xlabel ('Time (Milliseconds)','FontWeight', 'bold')
text(5000,1,'Positive Ytorque_a_v_g = 1.77','FontWeight','bold')

subplot(2,2,4);
plot(angular_velocities3)
ylabel ('Radians Per Second','FontWeight', 'bold')
xlabel ('Time In Milliseconds','FontWeight', 'bold')
text(5000,400,'Final Angular Velocity_1 = 0.00','FontWeight','bold')
text(5000,350,'Final Angular Velocity_2 = 0.00','FontWeight','bold')
text(5000,300,'Final Angular Velocity_3 = 616.26','FontWeight','bold')
text(5000,250,'Final Angular Velocity_4 = 0.00','FontWeight','bold')
text(5000,200,'Final Angular Velocity_5 = 621.37','FontWeight','bold')
text(5000,150,'Final Angular Velocity_6 = 0.00','FontWeight','bold')

%% No need to Plot Negative Values
%Neg Y TORQUE

a4 = readtable("ynegtorque.csv")

%csvread turns nans to 0s
t4 = csvread("ynegtorque.csv",1,0);

%code for processing pos z torque
y_negtorque = [t4(:,29)];
for i= 2:1:10343
    
    if (y_negtorque(i,1) == 0) 
      
       y_negtorque(i,1) = y_negtorque(i-1,1);

    end
    
end


%code for processing angular velocities force
angular_velocities4 = [t3(:,2:7)]
for k= 1:1:6    
    for i= 2:1:10343
        
        if (angular_velocities4(i,k) == 0) 
           
            angular_velocities4(i,k) = angular_velocities4(i-1,k);
    
        end
        
    end
end

ynegtorque_avg = mean( y_negtorque(1500:9500,1))
%val of -1.83





%%
% Solve for fc and tc using above values
%tests performed at 50% power
%need optimization toolbox


% given constants
l = 0.275;

% Measured values
Thrust = -11.6308;
tz = 0.0452;
ty = 1.77;
tx = 2.05;
tyn = -1.83;

syms tc fcx fcy fc_thrust negfc_y; 

%create arrays
tz_coeff = [-tc tc -tc tc tc -tc];
tz_weights = [0 626.21 0 636.23 616.19 0];

% equation
equation = sum(tz_coeff .* tz_weights) - tz;

% Solve for tc, convert to decimal with vpa
tc = vpa(solve(equation, tc),5)

%-------------------------------------------------
%ty solve for fc
ty_coeff = [0 0 (sqrt(3)*fcx*l)/2 -(sqrt(3)*fcx*l)/2  (sqrt(3)*fcx*l)/2  -(sqrt(3)*fcx*l)/2 ];
ty_weights = [0 0 616.26 0 621.37 0];

% equation
equation = sum(ty_coeff .* ty_weights) - ty;

% Solve for fc, convert to decimal with vpa
fc_y = vpa(solve(equation, fcx),5)

%-------------------------------------------------
%tx solve for fc
tx_coeff = [-fcx*l fcx*l (fcx*l)/2 -(fcx*l)/2 (fcx*l)/2 -(fcx*l)/2];
tx_weights = [0 628.75 617.68 0 0 620.31];

% equation
equation2 = sum(tx_coeff .* tx_weights) - tx;

% Solve for fc, convert to decimal with vpa
fc_x = vpa(solve(equation2, fcx),5)

%----------------------------------------------
% solve for fc using thrust
T_coeff = [fc_thrust fc_thrust fc_thrust fc_thrust fc_thrust fc_thrust ];
T_weights = [597.07 620.91 599.18 623.35 605.43 604.32];

% equation
equation3 = sum(T_coeff .* T_weights) - Thrust;

%----------------------------------------------
%neg torque fc_y
%create arrays
tyn_coeff = [0 0 (sqrt(3)*negfc_y*l)/2 -(sqrt(3)*negfc_y*l)/2  (sqrt(3)*negfc_y*l)/2  -(sqrt(3)*negfc_y*l)/2 ];
tyn_weights = [0 0 616.64 0 621.90 0];

% equation
equation4 = sum(tyn_coeff .* tyn_weights) - tyn;

% Solve for tc, convert to decimal with vpa
fc_y_neg = vpa(solve(equation4, negfc_y),5)



% Solve for fc, convert to decimal with vpa
%------------------------------------------
fc_T = vpa(solve(equation3, fc_thrust),5)

FC = [fc_x fc_y - fc_y_neg -fc_T ];

FC_Mean = mean(FC)

%test
sum (FC_Mean.* [300 300 300 300 300 300])
