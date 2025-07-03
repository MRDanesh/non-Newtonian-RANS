clc
clear all;

%--------------
imageSize = [500 355];
imageType = '-depsc';
% FONTS paper = 18, presentation = 22
FONTS = 25;
FONTW = 'normal';
FONTA = 'normal';
%--------------
%% Parameters
Bn=10;
n=0.8;
Re_tau = 323;
%*********Constants***********%
rho = 1000;
K=0.001;
D = 1;
L = 12;
R=D/2;
%********************

tau_w = (Re_tau * K^(1/n)/(R*(1-Bn/100)^(1/n)*rho^(1/2)))^(2*n/(2-n));
u_star = (tau_w/rho)^(1/2);
tau_y = (Bn/100)*tau_w;
g = (2*u_star^2)/R;
nu_w = (1/rho)*( (K^(1/n)) * tau_w )/ ((tau_w - tau_y)^(1/n));
lengthScale = nu_w/u_star;

%% Reading directory

fullFileName = 'data.csv';

%% Read file

data = readtable(fullFileName);
data=data{:,:};
x= data (:,14);
y= data (:,15);
uz=-data(:,10);

%%

N=600;
interpolant = scatteredInterpolant(x,y,uz,'linear','linear');
xx = linspace(-R,R,N);
yy = linspace(-R,R,N);
[xx,yy] = meshgrid(xx,yy);
uz = interpolant(xx,yy);

%% Set values outside the circle to NaN
mask = sqrt(xx.^2 + yy.^2) <= R;
uz(~mask) = NaN;   

 uz_radial = uz(N/2,1:N/2)/u_star;
 yPlus = (R+ xx(N/2,1:N/2))/lengthScale;

 plot(yPlus, uz_radial); hold on;
 set(gca, 'XScale', 'log')

% Create a table from your data
T = table(yPlus(:), uz_radial(:), 'VariableNames', {'yPlus','uv_radial'});

% Write the table to a CSV file named "myData.csv"
writetable(T, 'velocity_HB10_SSTHB.csv');
%% Save figure

x0=50;
y0=50;
width=380*2;
height=280*2;
set(gcf,'position',[x0,y0,width,height])
set(gca,'FontName','Times','FontSize',20,'LineWidth',3);
set(gca,'Position',[ 0.2 0.2 0.5 0.5]);

set(gcf,'InvertHardCopy','off')
set(gcf,'color','w');
ylim([0 30])
xlim([1 350])
