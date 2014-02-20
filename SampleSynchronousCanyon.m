pHistory=[];
cHistory=[];
M=[];

tcpIp = tcpip('192.168.0.2',26999, 'NetworkRole', 'client');
tcpIp.Timeout = Inf;
fopen(tcpIp);

startOfMessage = [hex2dec('12') hex2dec('34') hex2dec('56') hex2dec('78')];
senderId = [255, 255];
receiverId = [0,0];
sequenceNumber = [0,0,0,0];
syncConnectionRequestType = [0,1,0,4];
clientDataType = [0,3,0,5];
objectName = 'canyonview_apt';
padding = zeros(1,mod(length(objectName),4));
syncConnectionData = [objectName, padding];
syncConnectionDataSizeHex = dec2hex(length(syncConnectionData),8);
syncConnectionDataSize = [hex2dec(syncConnectionDataSizeHex(1:2)), hex2dec(syncConnectionDataSizeHex(3:4)), hex2dec(syncConnectionDataSizeHex(5:6)), hex2dec(syncConnectionDataSizeHex(7:8))];
endOfMessage = [hex2dec('FE') hex2dec('DC') hex2dec('BA') hex2dec('98')];

syncConnectionRequest = [startOfMessage,senderId,receiverId,sequenceNumber,syncConnectionRequestType,syncConnectionDataSize,syncConnectionData,endOfMessage];
fwrite(tcpIp,syncConnectionRequest);

syncConnectionResponse = fread(tcpIp,36);
senderId = syncConnectionResponse(7:8)'
requestResult = syncConnectionResponse(21:25);
if ( requestResult(4) == 0 )
    'Accepted'
else
    'Not Found'
end
systemTime = syncConnectionResponse(26:30);
systemTime = systemTime(1)*2^24+systemTime(2)*2^16+systemTime(3)*2^8+systemTime(4);
previousConsumption = ebu2Data(1)*100;
for i=1:192
setPriceMsg = fread(tcpIp,36);
'Set Price Received'
price = setPriceMsg(21:25);
price = price(1)*2^24+price(2)*2^16+price(3)*2^8+price(4)

if price > 500
    %consumption = (1000-price)*10000+1000000;
    consumption = ebu2Data(i)*(1380-price)/1000;
elseif price > 300
    %consumption = (1000-price*0.6)*10000+1000000;
    consumption = ebu2Data(i)*(1180-0.6*price)/1000;
else
    %consumption = (850)*10000+1000000;
    consumption = ebu2Data(i);
end

consumption=consumption + randn(1,1)*100

if consumption < 0 
    consumption = 1;
end

% if ( consumption - previousConsumption ) > 250000
%     consumption = previousConsumption + 250000;
% elseif consumption - previousConsumption < -250000
%     consumption = previousConsumption - 250000;
% end
% 
% previousConsumption = consumption;

%if i > 100 && i < 130
%    consumption = consumption + 10000000;
%end

%consumption = consumption + rand(1)*500000;

pHistory=[pHistory price];
cHistory=[cHistory consumption];
consumptionHex = dec2hex(uint32(consumption),8);
consumptionData = [hex2dec(consumptionHex(1:2)), hex2dec(consumptionHex(3:4)), hex2dec(consumptionHex(5:6)), hex2dec(consumptionHex(7:8))];
clientDataMsg = [startOfMessage,senderId,receiverId,sequenceNumber,clientDataType,syncConnectionDataSize,consumptionData,endOfMessage];
fwrite(tcpIp,clientDataMsg);
end
fclose(tcpIp);