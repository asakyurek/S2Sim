classdef OpenDSSManager < handle
    %OpenDSSManager Manages the interface with COM server
    %   This class is provided to abstract the COM server interface for
    %   OpenDSS control.
    
    properties(SetAccess='private', GetAccess='private')
        m_object
        m_circuit
        m_text
        m_solution
        m_loads
        
        m_tcpIp
        m_running
    end
    
    properties
        monitoredLoads;
        voltageResults;
        voltageCounter;
    end
    
    methods(Access='private')
        function result=GiveCommand(obj,command)
            obj.m_text.command = command;
            result = obj.m_text.result;
        end
    end
    
    methods
        function obj=OpenDSSManager()
            obj.m_object = actxserver('OpenDSSEngine.DSS');
            obj.m_object.Start(0);
            obj.m_circuit = obj.m_object.ActiveCircuit;
            obj.m_text = obj.m_object.Text;
            obj.CompileCircuit('C:\Users\Alper\Google Drive\Simulations\OpenDSSParser\UCSDParsed.DSS');
            obj.m_solution = obj.m_circuit.Solution;
            obj.m_loads = obj.m_circuit.Loads;
            obj.m_running = 0;
            obj.m_tcpIp = tcpip('192.168.0.2',26998, 'NetworkRole', 'client');
            obj.m_tcpIp.Timeout = Inf;
            obj.voltageResults = [];
            obj.voltageCounter = 1;
%            directory = 'C:\Program Files (x86)\Apache Software Foundation\Apache2.2\htdocs\python_codes\';
            %directory = 'C:\Users\Alper\Documents\smart_grid_demo\pyhton_codes\';
%             directory = 'C:\Users\Alper\Documents\smart_grid_demo\python_codes';
%             cd(directory);
%             delete('EBU2_power.csv');
%             delete('EBU2_voltage.csv');
%             delete('CVA_power.csv');
%             delete('CVA_voltage.csv')
%             delete('WA_power.csv');
%             delete('WA_voltage.csv')
%             delete('EBU2_percentage_power.csv');
%             delete('CVA_percentage_power.csv');
%             delete('WA_percentage_power.csv');
%             delete('EBU2_voltage_deviation.csv');
%             delete('CVA_voltage_deviation.csv');
%             delete('WA_voltage_deviation.csv');
%             delete('EBU2_power_one_liner.csv');
%             delete('EBU2_voltage_one_liner.csv');
%             delete('CVA_power_one_liner.csv');
%             delete('CVA_voltage_one_liner.csv')
%             delete('WA_power_one_liner.csv');
%             delete('WA_voltage_one_liner.csv')
%             delete('EBU2_percentage_power_one_liner.csv');
%             delete('CVA_percentage_power_one_liner.csv');
%             delete('WA_percentage_power_one_liner.csv');
%             delete('EBU2_voltage_deviation_one_liner.csv');
%             delete('CVA_voltage_deviation_one_liner.csv');
%             delete('WA_voltage_deviation_one_liner.csv');
%             cd '../../MATLAB'
        end
        function result=LoadExists(obj,objectName)
            result = 0;
            iLoad = obj.m_loads.First;
            while iLoad > 0
                if strcmp( objectName, obj.m_loads.Name )
                    result = 1;
                    break;
                end
                obj.m_loads.Name
                iLoad = obj.m_loads.Next;
            end
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
            fclose( obj.m_tcpIp );
            'OpenDSS Stopped'
        end
        function ProcessMessage( obj, message )
            messageType = message(1)*2^24 + message(2)*2^16 + message(3)*2^8 + message(4);
            if messageType == 1
                'Client Existance Message Received';
                clientName = char( message(5:end)' )
                if obj.LoadExists(clientName)
                    clientExistance = [0,0,0,1];
                    [clientName ' Exists']
                else
                    clientExistance = [0,0,0,2];
                    [clientName ' Does not exist']
                end
                responseMessageType = [0,0,0,2];
                sendingData = [responseMessageType, clientExistance];
                fwrite( obj.m_tcpIp, sendingData );
            elseif messageType == 3
                'Client Set Wattage Message Received';
                wattageValue = message(5)*2^24 + message(6)*2^16 + message(7)*2^8 + message(8);
                wattageValue = wattageValue / 1000
                clientName = char(message(9:end)');
                if strcmp( clientName, 'canyonview_apt' ) || strcmp( clientName, 'warren_apt' )
                    wattageValue = wattageValue * 250/0.72;
                end
                obj.SetWattage( clientName, wattageValue );
            elseif messageType == 4
                'Client Get Wattage Message Received'
                clientName = char(message(5:end)')
                wattageValue = obj.GetWattage( clientName )
                wattageHex = dec2hex(wattageValue,8);
                responseMessageType = [0,0,0,5];
                wattageData = [hex2dec(wattageHex(1:2)), hex2dec(wattageHex(3:4)), hex2dec(wattageHex(5:6)), hex2dec(wattageHex(7:8))];
                sendingData = [responseMessageType, wattageData];
                fwrite( obj.m_tcpIp, sendingData );
            elseif messageType == 6
                'Advance Time Step Received'
                obj.RunFor( 1 );
                
                %directory = 'C:\Program Files (x86)\Apache Software Foundation\Apache2.2\htdocs\python_codes\';
                %directory = 'C:\Users\Alper\Documents\smart_grid_demo\pyhton_codes\';
                %obj.voltageResults(obj.voltageCounter) = mean(abs(obj.GetVoltage('north_ebu2')));
                %timeValue = datestr(now());
                %timeValue(timeValue=='-')='/';
                
                
                ebu2WattageValue = 1000*obj.GetWattage('north_ebu2');
                ebu2VoltageValue = mean(abs(obj.GetVoltage('north_ebu2')));
                
                cvaWattageValue = 1000*obj.GetWattage('canyonview_apt');
                cvaVoltageValue = mean(abs(obj.GetVoltage('canyonview_apt')));
                
                waWattageValue = 1000*obj.GetWattage('warren_apt');
                waVoltageValue = mean(abs(obj.GetVoltage('warren_apt')));
                
                totalPower = ebu2WattageValue + cvaWattageValue + waWattageValue;
                ebu2PowerPercentage = (100*ebu2WattageValue/totalPower);
                cvaPowerPercentage = (100*cvaWattageValue/totalPower);
                waPowerPercentage = (100*waWattageValue/totalPower);
                
                if totalPower == 0
                    ebu2PowerPercentage = 0;
                    cvaPowerPercentage = 0;
                    waPowerPercentage = 0;
                end
                
                ebu2VoltageDeviation = 100-(100 * ebu2VoltageValue * sqrt(3) / 480);
                cvaVoltageDeviation = 100-(100 * cvaVoltageValue * sqrt(3) / 480);
                waVoltageDeviation = 100-(100 * waVoltageValue * sqrt(3) / 12470);
                
%                 directory = 'C:\Users\Alper\Documents\smart_grid_demo\python_codes';
%                 cd(directory);
%                 fHandle = fopen( 'EBU2_power.csv', 'a' );
%                 fwrite(fHandle,[datestr(now(),31) ',' num2str(ebu2WattageValue) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'EBU2_voltage.csv', 'a' );
%                 fwrite(fHandle,[datestr(now(),31) ',' num2str(ebu2VoltageValue) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'CVA_power.csv', 'a' );
%                 fwrite(fHandle,[datestr(now(),31) ',' num2str(cvaWattageValue) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'CVA_voltage.csv', 'a' );
%                 fwrite(fHandle,[datestr(now(),31) ',' num2str(cvaVoltageValue) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'WA_power.csv', 'a' );
%                 fwrite(fHandle,[datestr(now(),31) ',' num2str(waWattageValue) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'WA_voltage.csv', 'a' );
%                 fwrite(fHandle,[datestr(now(),31) ',' num2str(waVoltageValue) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'EBU2_percentage_power.csv', 'a' );
%                 fwrite(fHandle,[num2str(ebu2PowerPercentage) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'CVA_percentage_power.csv', 'a' );
%                 fwrite(fHandle,[num2str(cvaPowerPercentage) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'WA_percentage_power.csv', 'a' );
%                 fwrite(fHandle,[num2str(waPowerPercentage) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'EBU2_voltage_deviation.csv', 'a' );
%                 fwrite(fHandle,[num2str(ebu2VoltageDeviation) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'CVA_voltage_deviation.csv', 'a' );
%                 fwrite(fHandle,[num2str(cvaVoltageDeviation) 10]);
%                 fclose ( fHandle );
%                 fHandle = fopen( 'WA_voltage_deviation.csv', 'a' );
%                 fwrite(fHandle,[num2str(waVoltageDeviation) 10]);
%                 fclose ( fHandle );
%                 cd ../../MATLAB
                
                obj.voltageCounter = obj.voltageCounter + 1;
            elseif messageType == 7
                'Client Get Voltage Message Received'
                clientName = char(message(5:end)')
                voltageValue = mean(abs(obj.GetVoltage( clientName )))
                voltageHex = dec2hex(uint32(voltageValue*32768),8);
                responseMessageType = [0,0,0,8];
                voltageData = [hex2dec(voltageHex(1:2)), hex2dec(voltageHex(3:4)), hex2dec(voltageHex(5:6)), hex2dec(voltageHex(7:8))];
                sendingData = [responseMessageType, voltageData];
                fwrite( obj.m_tcpIp, sendingData );
            end
        end
        function result=CompileCircuit(obj,fileName)
            result = obj.GiveCommand(['Compile (' fileName ')']);
        end
        function result=SetStepSize(obj,stepSize)
            obj.m_solution.StepSize = stepSize;
            result = obj.m_solution.StepSize;
        end
        function [runTime,voltages,currents,powers]=RunFor(obj,time)
            runNumber = floor( time/obj.m_solution.StepSize );
            monitoredNumber = size(obj.monitoredLoads,1);
            voltages = zeros(monitoredNumber,runNumber,3);
            currents = zeros(monitoredNumber,runNumber,3);
            powers = zeros(monitoredNumber,runNumber,3);
            for runInstance = 1:runNumber
                obj.m_solution.Solve;
                for monitorIndex = 1:monitoredNumber
                    loadName = obj.monitoredLoads(monitorIndex,:);
                    voltages(monitorIndex,runInstance,:)=obj.GetVoltage(loadName);
                    currents(monitorIndex,runInstance,:)=obj.GetCurrent(loadName);
                    powers(monitorIndex,runInstance,:)=obj.GetPower(loadName);
                end
            end
            runTime = runNumber*obj.m_solution.StepSize;
        end
        function obj=SetWattage(obj,loadName,wattage)
            iLoad = obj.m_loads.First;
            while iLoad > 0
                if strcmp( loadName, obj.m_loads.Name )
                    obj.m_loads.kW=wattage;
                    break;
                end
                iLoad = obj.m_loads.Next;
            end
        end
        function wattage=GetWattage(obj,loadName)
            iLoad = obj.m_loads.First;
            while iLoad > 0
                if strcmp( loadName, obj.m_loads.Name )
                    wattage = obj.m_loads.kW;
                    return;
                end
                iLoad = obj.m_loads.Next;
            end
            error([loadName ' not found']);
        end
        function voltage=GetVoltage(obj,loadName)
            iLoad = obj.m_loads.First;
            while iLoad > 0
                if strcmp( loadName, obj.m_loads.Name )
                    tempVoltage=obj.m_circuit.ActiveCktElement.Voltages;
                    voltage=[tempVoltage(1)+1i*tempVoltage(2),...
                             tempVoltage(3)+1i*tempVoltage(4),...
                             tempVoltage(5)+1i*tempVoltage(6)];
                    return;
                end
                iLoad = obj.m_loads.Next;
            end
            error([loadName ' not found']);
        end    
        function current=GetCurrent(obj,loadName)
            iLoad = obj.m_loads.First;
            while iLoad > 0
                if strcmp( loadName, obj.m_loads.Name )
                    tempCurrent=obj.m_circuit.ActiveCktElement.Currents;
                    current=[tempCurrent(1)+1i*tempCurrent(2),...
                             tempCurrent(3)+1i*tempCurrent(4),...
                             tempCurrent(5)+1i*tempCurrent(6)];
                    return;
                end
                iLoad = obj.m_loads.Next;
            end
            error([loadName ' not found']);
        end
        function power=GetPower(obj,loadName)
            iLoad = obj.m_loads.First;
            while iLoad > 0
                if strcmp( loadName, obj.m_loads.Name )
                    tempPower=obj.m_circuit.ActiveCktElement.Powers;
                    power=[tempPower(1)+1i*tempPower(2),...
                           tempPower(3)+1i*tempPower(4),...
                           tempPower(5)+1i*tempPower(6)];
                    return;
                end
                iLoad = obj.m_loads.Next;
            end
            error([loadName ' not found']);
        end
    end    
end

