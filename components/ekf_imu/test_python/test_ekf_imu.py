import numpy as np
import pandas as pd
import serial
import io
import struct


class SimRunner():
    def __init__(self, sim_datafile):
        self.sim_datafile = sim_datafile
        self.sim_df = pd.read_csv(sim_datafile)
        print("Please enter the serial port")
        self.serial_port = input()
        self.sim_data_in = []

    def end_sim(self):
        sim_results_df = pd.DataFrame(self.sim_data_in)
        print(self.sim_data_in)
        sim_results_df.to_csv("sim_output.txt")
        print("output simulation results to sim_output.txt")

    def run_sim(self):
        with serial.Serial(self.serial_port, 115200, timeout=100) as ser:
            print("Beginning simulation")
            ser.reset_input_buffer()
            ser.write(b'bg')
            data_index = 0
            while data_index < 298:
                self.last_line = ser.readline()
                if (self.last_line[0:2] == ("re").encode('ascii')):
                    print("received request")
                    sim_data_out = []
                    for fl_data in self.sim_df.iloc[data_index,:6]:
                        print(fl_data)
                        for byte in struct.pack("!f", fl_data):
                            print(byte)
                            sim_data_out.append(byte)
                    ser.write(sim_data_out)
                    data_index+=1
                elif (self.last_line == ("w\n").encode('ascii')):
                    print("received sim info")
                    index = 0
                    sim_data_in_raw = []
                    while (index < 7):
                        self.last_line = ser.readline()
                        print(self.last_line)
                        sim_data_in_raw.append(self.last_line.rstrip(b'm\n'))
                        index += 1
                        #new_row_bytes = sim_data_in_raw.split((',').encode('utf-8'))
                    new_row_fl = []
                    for raw_data in sim_data_in_raw:
                        print(raw_data)
                        try:
                            print(struct.unpack('f', raw_data))
                            new_row_fl.append(struct.unpack('f', raw_data))
                        except: 
                            new_row_fl.append(None)
                    self.sim_data_in.append(new_row_fl)
                        
                else:
                    print("received: ")
                    print(self.last_line)
                    print(self.last_line[0:2])
            print("Ending Simulation")
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
