clc
clear all

data1 = readtable('velocity_HB10_SST.csv');
data1=data1{:,:};
time1=data1(:,1);
vel1= data1 (:,2);

data2 = readtable('velocity_HB10_SSTHB.csv');
data2=data2{:,:};
time2=data2(:,1);
vel2= data2 (:,2);
% 
data3 = readtable('reference_velocity_HB10.csv');
data3=data3{:,:};
time3=data3(:,1);
vel3= data3 (:,2);

plot(time1, vel1, 'Linewidth',2, 'Color',[0 0 0]); hold on
plot(time2, vel2, "--", 'Linewidth',2, 'Color',[0 0 0]); hold on
scatter(time3, vel3,80,  's','MarkerEdgeColor', 'black','Linewidth',1); 
hold on;

set(gca, 'XScale', 'log')
xlabel('$y^+$','Interpreter','latex');
ylabel('$U_z^+$','Interpreter','latex');
    
    
    legend_str = { 'SST',  'SSTHB',  'DNS', 'Newt (DNS)', 'HB10 (DNS)', 'Bn20 (DNS)'};
legHandle=legend(legend_str,'Location','eastoutside');
% % 
  set(legHandle,'Interpreter','Latex','FontName','Times','FontSize',16,'NumColumns',1,...
          'LineWidth',3);

 set(legHandle, 'Position', [0.205, 0.445, 0.2, 0.2]); % Modify values as needed

%% Save figure

x0=50;
y0=50;
width=450*2;
height=280*2;
set(gcf,'position',[x0,y0,width,height])
set(gca,'FontName','Times','FontSize',22,'LineWidth',3);
set(gca,'Position',[ 0.2 0.2 0.5 0.5]);


% view(270,90);
set(gcf,'InvertHardCopy','off')
set(gcf,'color','w');
ylim([0 25])
xlim([1 350])

% Save as PDF
print('./validation_meanVelocityHB10', '-dpdf', '-painters');
