classdef ControlManager < handle
    %ControlManager Implements a simple control algorithm
    %   This class is provided to implement a basic control algorithm based
    % on pricing.
    
    properties(SetAccess='private', GetAccess='public')
        m_tcpIp
        m_running
        m_numberOfClients
        m_systemMode
        m_systemTime
        m_clientIds
        m_clientPrices
        m_voltageHistory
    end
    
    methods
        function obj=ControlManager()
            obj.m_running = 0;
            obj.m_tcpIp = tcpip('192.168.0.2',26997, 'NetworkRole', 'client','TransferDelay','off' );
            obj.m_tcpIp.Timeout = Inf;
        end
        function Run( obj )
            obj.m_running = 1;
            while ( obj.m_running )
                if strcmp( obj.m_tcpIp.Status, 'closed' )
                    connectionWaiting = 1;
                    while connectionWaiting && obj.m_running
                        try
                            fopen( obj.m_tcpIp );
                            connectionWaiting = 0;
                            'Connected!'
                            fwrite( obj.m_tcpIp, [0,0,0,2] );
                        catch
                            'No connection, retrying...'
                        end
                    end
                end
                while ( obj.m_running )
                    sizeArray = fread( obj.m_tcpIp, 4 );
                    if isempty( sizeArray )
                        fclose( obj.m_tcpIp );
                        break;
                    end
                    messageSize = sizeArray(1)*2^24 + sizeArray(2)*2^16 + sizeArray(3)*2^8 + sizeArray(4);
                    message = fread( obj.m_tcpIp, messageSize - 4 );
                    if size( message ) > 0
                        obj.ProcessMessage( message );
                    end
                end
            end
        end
        function Stop( obj )
            obj.m_running = 0;
            obj.m_voltageHistory=[];
            fclose( obj.m_tcpIp );
            'Control Manager Stopped'
        end
        function ProcessMessage( obj, message )
            messageType = message(1)*2^24 + message(2)*2^16 + message(3)*2^8 + message(4);
            if messageType == 1
                'Make Decission Message Received';
                obj.m_numberOfClients = message(5)*2^8 + message(6);
                obj.m_systemMode = message(7)*2^8 + message(8);
                obj.m_systemTime = message(9)*2^24 + message(10)*2^16 + message(11)*2^8 + message(12);
                clientTable = reshape( message(13:end), 8, obj.m_numberOfClients )';
                obj.m_clientIds = clientTable(:,1)*2^8+clientTable(:,2);
                clientIds = clientTable(:,3)*2^8+clientTable(:,4)
                clientVoltages = (clientTable(:,5)*2^24 + clientTable(:,6)*2^16 + clientTable(:,7)*2^8+clientTable(:,8))/32768
                
                stableVoltages = abs( clientVoltages * sqrt(3) - 480 ) > abs( clientVoltages * sqrt(3) - 12470 );
                stableVoltages = 12470*stableVoltages + 480*(~stableVoltages)
                
                voltageDeviations = ( ( stableVoltages - clientVoltages * sqrt(3) )./stableVoltages )
                
                windowSize = 20;
                
                if size(obj.m_voltageHistory,2) > obj.m_numberOfClients
                    obj.m_voltageHistory=obj.m_voltageHistory(1:obj.m_numberOfClients,:);
                end
                if isempty(obj.m_voltageHistory)
                    obj.m_voltageHistory=[];
                end
                
                if size(obj.m_voltageHistory,2) >= windowSize
                    obj.m_voltageHistory(:,1:(windowSize-1))=obj.m_voltageHistory(:,2:windowSize);
                else                   
                    obj.m_voltageHistory(:,size(obj.m_voltageHistory,2)+1)=voltageDeviations;
                end
                previousPrices = obj.m_clientPrices;
                obj.m_clientPrices = 5000 * mean(obj.m_voltageHistory,2);
                clientPrices = obj.m_clientPrices
                if size(previousPrices) == size(obj.m_clientPrices)
                    if obj.m_clientPrices > previousPrices + 350
                        obj.m_clientPrices = previousPrices + 350;
                    elseif obj.m_clientPrices < previousPrices - 350
                        obj.m_clientPrices = previousPrices - 350;
                    end
                end
                
                for i=1:obj.m_numberOfClients      
                    priceHex = dec2hex(uint32(obj.m_clientPrices(i)),8);
                    priceData = [hex2dec(priceHex(1:2)), hex2dec(priceHex(3:4)), hex2dec(priceHex(5:6)), hex2dec(priceHex(7:8))];
                    responseMessageType = [0,0,0,3];
                    sendingData = [responseMessageType, clientTable(i,1:4), priceData]
                    'Sending Price'
                    fwrite( obj.m_tcpIp, sendingData )
                end
                
                'Sending Frame End'
                fwrite( obj.m_tcpIp, [0,0,0,2] );
            elseif messageType == 5
                'Client Price Request Received';
                clientId = message(5)*2^8 + message(6);
                price = obj.m_clientPrices(obj.m_clientIds == clientId);
                priceHex = dec2hex(price,8);
                priceData = [hex2dec(priceHex(1:2)), hex2dec(priceHex(3:4)), hex2dec(priceHex(5:6)), hex2dec(priceHex(7:8))];
                responseMessageType = [0,0,0,3];
                sendingData = [responseMessageType, message(5:6), priceData];
                'Sending Price'
                fwrite( obj.m_tcpIp, sendingData );
            elseif messageType == 6
                'Client Demand Negotiation Received: Not Implemented'
                clientId = message(5)*8 + message(6);
            end
        end
    end    
end

