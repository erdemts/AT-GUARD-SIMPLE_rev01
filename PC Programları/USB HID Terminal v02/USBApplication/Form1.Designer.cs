namespace USBApplication
{
    partial class Form1
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.components = new System.ComponentModel.Container();
            this.microgeniosUsbHID_Port1 = new microgeniosUsbHID.microgeniosUsbHID_Port(this.components);
            this.lblStatusUSB = new System.Windows.Forms.Label();
            this.textBox1 = new System.Windows.Forms.TextBox();
            this.btnSend = new System.Windows.Forms.Button();
            this.textBox2 = new System.Windows.Forms.TextBox();
            this.timer1 = new System.Windows.Forms.Timer(this.components);
            this.button1 = new System.Windows.Forms.Button();
            this.txtCard = new System.Windows.Forms.TextBox();
            this.btnAddCard = new System.Windows.Forms.Button();
            this.btnDelCard = new System.Windows.Forms.Button();
            this.btnCheckCard = new System.Windows.Forms.Button();
            this.txtTip = new System.Windows.Forms.TextBox();
            this.SuspendLayout();
            // 
            // microgeniosUsbHID_Port1
            // 
            this.microgeniosUsbHID_Port1.UsbHID_ProductId = 0;
            this.microgeniosUsbHID_Port1.UsbHID_VendorId = 0;
            this.microgeniosUsbHID_Port1.microgeniosDeviceUsbHID_Encontrado += new System.EventHandler(this.microgeniosUsbHID_Port1_microgeniosDeviceUsbHID_Encontrado);
            this.microgeniosUsbHID_Port1.microgeniosDeviceUsbHID_Removido += new System.EventHandler(this.microgeniosUsbHID_Port1_microgeniosDeviceUsbHID_Removido);
            this.microgeniosUsbHID_Port1.microgeniosDeviceUsbHID_ReceberDados += new microgeniosUsbHID.DataRecievedEventHandler(this.microgeniosUsbHID_Port1_microgeniosDeviceUsbHID_ReceberDados);
            // 
            // lblStatusUSB
            // 
            this.lblStatusUSB.AutoSize = true;
            this.lblStatusUSB.Location = new System.Drawing.Point(12, 9);
            this.lblStatusUSB.Name = "lblStatusUSB";
            this.lblStatusUSB.Size = new System.Drawing.Size(0, 13);
            this.lblStatusUSB.TabIndex = 0;
            // 
            // textBox1
            // 
            this.textBox1.Location = new System.Drawing.Point(12, 25);
            this.textBox1.Name = "textBox1";
            this.textBox1.Size = new System.Drawing.Size(344, 20);
            this.textBox1.TabIndex = 1;
            // 
            // btnSend
            // 
            this.btnSend.Location = new System.Drawing.Point(281, 51);
            this.btnSend.Name = "btnSend";
            this.btnSend.Size = new System.Drawing.Size(75, 23);
            this.btnSend.TabIndex = 2;
            this.btnSend.Text = "Gönder";
            this.btnSend.UseVisualStyleBackColor = true;
            this.btnSend.Click += new System.EventHandler(this.btnSend_Click);
            // 
            // textBox2
            // 
            this.textBox2.Location = new System.Drawing.Point(9, 80);
            this.textBox2.Multiline = true;
            this.textBox2.Name = "textBox2";
            this.textBox2.Size = new System.Drawing.Size(347, 274);
            this.textBox2.TabIndex = 3;
            // 
            // timer1
            // 
            this.timer1.Tick += new System.EventHandler(this.timer1_Tick);
            // 
            // button1
            // 
            this.button1.Location = new System.Drawing.Point(446, 360);
            this.button1.Name = "button1";
            this.button1.Size = new System.Drawing.Size(75, 23);
            this.button1.TabIndex = 4;
            this.button1.Text = "SİL";
            this.button1.UseVisualStyleBackColor = true;
            this.button1.Click += new System.EventHandler(this.button1_Click);
            // 
            // txtCard
            // 
            this.txtCard.Location = new System.Drawing.Point(403, 25);
            this.txtCard.Name = "txtCard";
            this.txtCard.Size = new System.Drawing.Size(260, 20);
            this.txtCard.TabIndex = 5;
            // 
            // btnAddCard
            // 
            this.btnAddCard.Location = new System.Drawing.Point(426, 51);
            this.btnAddCard.Name = "btnAddCard";
            this.btnAddCard.Size = new System.Drawing.Size(75, 23);
            this.btnAddCard.TabIndex = 6;
            this.btnAddCard.Text = "Kart Ekle";
            this.btnAddCard.UseVisualStyleBackColor = true;
            this.btnAddCard.Click += new System.EventHandler(this.btnAddCard_Click);
            // 
            // btnDelCard
            // 
            this.btnDelCard.Location = new System.Drawing.Point(507, 51);
            this.btnDelCard.Name = "btnDelCard";
            this.btnDelCard.Size = new System.Drawing.Size(75, 23);
            this.btnDelCard.TabIndex = 7;
            this.btnDelCard.Text = "Kart Sil";
            this.btnDelCard.UseVisualStyleBackColor = true;
            this.btnDelCard.Click += new System.EventHandler(this.btnDelCard_Click);
            // 
            // btnCheckCard
            // 
            this.btnCheckCard.Location = new System.Drawing.Point(588, 51);
            this.btnCheckCard.Name = "btnCheckCard";
            this.btnCheckCard.Size = new System.Drawing.Size(75, 23);
            this.btnCheckCard.TabIndex = 8;
            this.btnCheckCard.Text = "Kart Kontrol Et";
            this.btnCheckCard.UseVisualStyleBackColor = true;
            this.btnCheckCard.Click += new System.EventHandler(this.btnCheckCard_Click);
            // 
            // txtTip
            // 
            this.txtTip.Location = new System.Drawing.Point(669, 25);
            this.txtTip.Name = "txtTip";
            this.txtTip.Size = new System.Drawing.Size(62, 20);
            this.txtTip.TabIndex = 9;
            this.txtTip.Text = "1";
            // 
            // Form1
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.ClientSize = new System.Drawing.Size(885, 383);
            this.Controls.Add(this.txtTip);
            this.Controls.Add(this.btnCheckCard);
            this.Controls.Add(this.btnDelCard);
            this.Controls.Add(this.btnAddCard);
            this.Controls.Add(this.txtCard);
            this.Controls.Add(this.button1);
            this.Controls.Add(this.textBox2);
            this.Controls.Add(this.btnSend);
            this.Controls.Add(this.textBox1);
            this.Controls.Add(this.lblStatusUSB);
            this.Name = "Form1";
            this.Text = "Form1";
            this.Activated += new System.EventHandler(this.Form1_Activated);
            this.ResumeLayout(false);
            this.PerformLayout();

        }

        #endregion

        private microgeniosUsbHID.microgeniosUsbHID_Port microgeniosUsbHID_Port1;
        private System.Windows.Forms.Label lblStatusUSB;
        private System.Windows.Forms.TextBox textBox1;
        private System.Windows.Forms.Button btnSend;
        private System.Windows.Forms.TextBox textBox2;
        private System.Windows.Forms.Timer timer1;
        private System.Windows.Forms.Button button1;
        private System.Windows.Forms.TextBox txtCard;
        private System.Windows.Forms.Button btnAddCard;
        private System.Windows.Forms.Button btnDelCard;
        private System.Windows.Forms.Button btnCheckCard;
        private System.Windows.Forms.TextBox txtTip;
    }
}

