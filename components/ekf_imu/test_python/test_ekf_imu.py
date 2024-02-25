import numpy as np
import pandas as pd
import serial

sim_df = pd.dataframe()

class SimRunner():
    def __init__(self, sim_datafile):
        self.sim_datafile = sim_datafile
        self.sim_df = pd.read_csv(sim_datafile)
        print("Please enter the serial port")
        self.serial_port = input()
        self.sim_data_in = []

    def end_sim(self):
        sim_results_df = pd.DataFrame(self.sim_data_in)
        sim_results_df.to_csv(output_datafile)

    def run_sim(self):
        with serial.Serial(serial_port, 115200, timeout=100) as ser:
            data_index = 0
            self.last_line = ser.readline()
            if (self.last_line[0:1] == 'ra'):
                ser.write(self.sim_df[data_index,0:2])
                data_index+=1
            elif (self.last_line[0:1] == 'rg')
                ser.write(self.sim_df[data_index,3:5])
                data_index+=1
            elif (self.last_line[0] == 'w')
                sim_data_in = ser.readline()
                new_row = sim_data_in.split(',')
                self.sim_data_in.append(new_row)
            elif (self.last_line[0] == 'e')
                self.end_sim()


def main(args=None):

    # TODO: After the simulation data is available inside of a df we will try to
    # initialize the serial connection. The target device will be set up to wait
    # for a serial connection. Once the serial connection is setup the target will
    # poll this program to ask if it's ready to transmit. Upon receiving back a 
    # ready signal, the target will begin polling for the simulated sensor data.
    # Once the data is received by the target it will process the data. Every time
    # the Kalman filter is updated the device will attempt to log the state info,
    # which will be sent back to the host computer with a signifier to let the host
    # know the target is attempting to write to it.
    
    sim_runner = SimRunner("./test_data/ekf_imu_sim_2-23-24.txt")
    sim_runner.run_sim()
    
    #init_serial_connection()
    

if __name__ == '__main__':
    main()
