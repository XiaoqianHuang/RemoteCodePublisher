///////////////////////////////////////////////////////////////////////
// SourceCodeWindow.xmal.cs - window for displaying the source code 
// them either in browser or new window.
// Language:    C++, Visual Studio 2019   
// Platform:      Macbook Pro , Win 10  
// Application: Project #3 - Code Publisher Client - CSE687
// Author:         Xiaoqian Huang (SU ID: 878174727)         
//                        xhuang62@syr.edu
///////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package is responsible for providing window for displaying the
* source code 
*
* Required Files:
* ---------------
* /
*
* Maintenance History:
* --------------------
* ver 1.0 : 4 Feb 2019
* - first release
* - demonstrate the application meet all the requiremnet in project#3
*
* Notes:
* ------
* - Designed to provide all functionality in header file.
* - Implementation file only needed for test and demo.
*
* Planned Additions and Changes:
* ------------------------------
* - More exception handle
*/

using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Shapes;

namespace WPF
{
    public partial class SourceCodeWindow : Window
    {
        //----< constructor >---------------------------------------------
        public SourceCodeWindow()
        {
            InitializeComponent();
        }
    }
}
