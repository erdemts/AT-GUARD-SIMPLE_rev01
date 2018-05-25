using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows.Forms;
using microgeniosUsbHID;
using System.IO;

namespace USBApplication
{
    //STX KOMUT DATASIZE DATA CRC ETX

    public partial class Form1 : Form
    {
        byte[] DataWrite = new byte[64];
        byte[] DataRead = new byte[64];

        TextWriter tw;
        int sayac = 1;

        byte STX = 0x0F;
        byte ETX = 0x04;
        byte DLE = 0x05;
        public Form1()
        {
            InitializeComponent();
            microgeniosUsbHID_Port1.SerialNumber = "12345";
            
        }

      

        private void Form1_Activated(object sender, EventArgs e)
        {
            microgeniosUsbHID_Port1.DeviceUsbHID_Init(0x04d8, 0x0003f);
        }

        private void microgeniosUsbHID_Port1_microgeniosDeviceUsbHID_Encontrado(object sender, EventArgs e)
        {
            lblStatusUSB.Text = "Device found";
        }

        private void microgeniosUsbHID_Port1_microgeniosDeviceUsbHID_Removido(object sender, EventArgs e)
        {
            lblStatusUSB.Text = "Device not found";
        }

        protected override void OnHandleCreated(EventArgs e)
        {
            base.OnHandleCreated(e);
            microgeniosUsbHID_Port1.RegistrarHandle(Handle);
        }

        protected override void WndProc(ref Message m)
        {
            microgeniosUsbHID_Port1.UsbHIDMensagens(ref m);
            base.WndProc(ref m);
        }

      

        private void SendData(byte command, byte[] data)
        {
            List<byte> temp = new List<byte>();

            short serialNumber = short.Parse("12345");
            byte serialNumberH = (byte)((serialNumber >> 8) & 0x00FF );
            byte serialNumberL = (byte)(serialNumber & 0x00FF);
            temp.Add(STX);
            //temp.Add(serialNumberH);
            //temp.Add(serialNumberL);
            temp.Add(command);
            temp.Add((byte)data.Length);

            for (int i = 0; i < data.Length; ++i)
            {
                temp.Add(data[i]);
            }
            temp.RemoveAt(0);
            temp.AddRange(GetCrc(temp.ToArray()));
            temp.Insert(0, STX);
            temp.Add(ETX);

            for (int i = 0; i < temp.Count; ++i)
            {
                //if (i != 0 && i != temp.Count - 1)
                //{
                //    if (temp[i] == STX || temp[i] == ETX || temp[i] == DLE)
                //    {
                //        DataWrite[count++] = DLE;
                //    }
                //}
                DataWrite[i] = temp[i];
            }

            
            microgeniosUsbHID_Port1.DeviceUsbHID.EnviarDados(DataWrite);
        }

        private byte[] GetCrc(byte[] values)
        {
            ushort rv = 0;
            byte[] returnval = new byte[2];
            foreach (byte crc in values)
            {
                rv = (ushort)((rv >> 8) | (rv << 8));
                rv ^= (ushort)(crc);
                rv ^= (ushort)((rv & 0xFF) >> 4);
                rv ^= (ushort)((rv << 8) << 4);
                rv ^= (ushort)(((rv & 0xff) << 4) << 1);
            }

            returnval[0] = (byte)((rv >> 8) & 0xFF);
            returnval[1] = (byte)(rv & 0xFF);
            return returnval;
        }

        private bool CheckCrc(byte[] buffer)
        {
            List<byte> Data = new List<byte>();

            for (int i = 0; i < buffer.Length - 2; i++)
            {
                Data.Add(buffer[i]);
            }
            byte[] Crc = GetCrc(Data.ToArray());

            return (buffer[buffer.Length - 2] == Crc[0] && buffer[buffer.Length - 1] == Crc[1]);
        }

        private void btnSend_Click(object sender, EventArgs e)
        {
            if (this.textBox1.Text == "") return;

            string[] values = textBox1.Text.Split(',');
            List<byte> data = new List<byte>();
            byte tryParse;

            foreach (string val in values)
            {
                if (!byte.TryParse(val, out tryParse)) return;
                
            }

            byte command = byte.Parse(values[0]);
            for (int i = 1; i < values.Length; ++i)
            {
                data.Add(byte.Parse(values[i]));
            }

            SendData(command, data.ToArray());
            
        }

        private void microgeniosUsbHID_Port1_microgeniosDeviceUsbHID_ReceberDados(object sender, DataRecievedEventArgs args)
        {
           // System.Threading.Thread.Sleep(20);
            DataRead = args.data;
            InvokeData(DataRead);
        }

        private void InvokeData(byte[] data)
        {
            if (data[0] == STX)
            {
                var command = data[1];
                var datasize = data[2];
                var datas = new byte[datasize];
                for (int i = 0; i < datasize; ++i)
                    datas[i] = data[i + 3];
                var crch = data[3 + datasize];
                var crcl = data[4 + datasize];
                var array = new byte[5 + datasize];
                array[0] = command;
                array[1] = datasize;
                for (int i = 0; i < datasize; ++i)
                {
                    array[2 + i] = datas[i];
                }
                array[3 + datasize] = crch;
                array[4 + datasize] = crcl;
                if (command == 208)
                {
                    SendData(108, new byte[] { });
                    foreach (byte by in datas)
                        this.textBox2.Invoke(new Action(() => this.textBox2.Text += by.ToString() + "  "));
                }
                else if (command == 209)
                {
                    SendData(108, new byte[] { });
                    WriteToText(array);
                }
                else if (command == 210)
                {

                }
                else
                {
                    foreach (byte by in datas)
                    this.textBox2.Invoke(new Action(() => this.textBox2.Text += by.ToString() + "  "));
                }
            }
        }
       
        private void WriteToText(byte[] data)
        {
            tw.Write(sayac++ + ")   ");
            foreach (byte b in data)
            {
                tw.Write( b + "\t");
            }
            tw.WriteLine();
           
        }

        private void timer1_Tick(object sender, EventArgs e)
        {

        }

        private void button1_Click(object sender, EventArgs e)
        {
            this.textBox2.Text = "";
        }

        private void button2_Click(object sender, EventArgs e)
        {
            tw = new StreamWriter("result1.txt");
            sayac = 1;
            SendData((byte)107, new byte[]{});
            
        }

        private void button3_Click(object sender, EventArgs e)
        {
            tw.WriteLine();
            tw.WriteLine("**********************************************EOF**********************************************");
            tw.Close();
            System.Diagnostics.Process.Start("result1.txt");
        }
    }
}
