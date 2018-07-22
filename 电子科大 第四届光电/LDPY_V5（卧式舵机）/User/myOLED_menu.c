

	#include "myOLED_menu.h"
		

	extern CCD_Info CCD2_info;
		
	extern Steer_Info Steer_info;
	extern Speed_Info Speed_info;
	extern Motor_Info Motor_info;
	extern Car_Mode Car_mode;
	extern Road_Type Road_type;
	
	extern Gyro_Info Gyro_info;
	
	extern HeadSteer_Info HeadSteer_info;
	
	extern Parameter_Info Parameter_info;
	
	#ifdef RemRoad_Control_Enable
	
		extern RemSpeedUpDown_Info RemSpeedUpDown_info;
		
	#endif
	
 uint8_t lcd_menu_display_init(menu * Menu)
  {
	  
		Menu->Ready_Go =  1;

		switch(Menu->choice_flag / 100)
		{
			
		    case 0: if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}
							myOLED_String(4,20,"Ling-Du-Pian-Yi");
						    break;
			case 1:   //Sudu1		
					{					
				      if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}			        
							myOLED_String(4,1,"*");
							myOLED_String(6,10,"Binay");myOLED_String(6,50,"Zhi");myOLED_Dec(6,87,zhi);
							myOLED_String(4,10,"SuDu1");myOLED_String(5,50,"Wan");myOLED_Dec(5,87,wan);
							myOLED_String(2,10,"SuDu2");myOLED_String(4,50,"X-S");myOLED_Dec(4,87,x_s);
														myOLED_String(3,50,"Up");myOLED_Dec(3,87,Up);
														myOLED_String(2,50,"Dn");myOLED_Dec(2,87,Dn);
														myOLED_String(1,50,"Ren");myOLED_Dec(1,87,Ren);
							switch(Menu->choice_flag % 100 / 10)
							{
								case 1:  //第二层	直道	速度									 												 
								          myOLED_String(6,40,"*");
													switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //
																		 myOLED_String(6,77,"*");
																		 switch(Menu->add_sub)
																		 {
																		   case 0:
																				       break;
																			 case 1: 
																				       zhi += 2;
																					
																			         myOLED_String(6,121,"+");
																			         
																							 Menu->add_sub = 0;
																			         break;
																			 case 2: 
																						if(zhi >= 2)
																						{
																							zhi -= 2;
																						}
																			       
																							 myOLED_String(6,121,"-");
																							 Menu->add_sub = 0;
																			         break;
																		 }
															     break;
													 }
												 break;
								 case 2: //第二层	弯道	速度								 											  
												 myOLED_String(5,40,"*");										 
												 switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //					
								                     myOLED_String(5,77,"*");	
																		 switch(Menu->add_sub)
																		 {
																		   case 0:
																				       break;
																			 case 1: 
																				       wan += 2;
																			         myOLED_String(5,121,"+");																			         
																							 Menu->add_sub = 0;
																			         break;
																			 case 2: 
																					if(wan >= 2)
																					{
																				       wan -= 2;
																					}
																							 myOLED_String(5,121,"-");
																							 Menu->add_sub = 0;
																			         break;
																		 }
															     break;
													 }
												 break;
									case 3:  ////第二层	小S 速度
												  myOLED_String(4,40,"*");										 
												  switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //					
								                     myOLED_String(4,77,"*");	
																		 switch(Menu->add_sub)
																		 {
																		   case 0:
																				       break;
																			 case 1: 
																				       x_s += 2;
																			         myOLED_String(4,121,"+");																			         
																							 Menu->add_sub = 0;
																			         break;
																			 case 2: 
																					if(x_s >= 2)
																					{
																				       x_s -= 2;
																					}
																							 myOLED_String(4,121,"-");
																							 Menu->add_sub = 0;
																			         break;
																		 }
															     break;
													 }
												 break;
								  case 4:  ////第二层	//弯道最大偏差
												  myOLED_String(3,40,"*");										 
												  switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //					
								                     myOLED_String(3,77,"*");	
																		 switch(Menu->add_sub)
																		 {
																		   case 0:
																				       break;
																			 case 1: 
																				       Up += 2;
																			         myOLED_String(3,121,"+");																			         
																							 Menu->add_sub = 0;
																			         break;
																			 case 2: 
																					if(Up >= 2)
																					{
																				       Up -= 2;
																					}
																							 myOLED_String(3,121,"-");
																							 Menu->add_sub = 0;
																			         break;
																		 }
															     break;
													 }
												 break;
								case 5:  ////第二层	//小S最大变化率
												  myOLED_String(2,40,"*");										 
												  switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //					
								                     myOLED_String(2,77,"*");	
																		 switch(Menu->add_sub)
																		 {
																		   case 0:
																				       break;
																			 case 1: 
																				       Dn += 2;
																			         myOLED_String(2,121,"+");																			         
																							 Menu->add_sub = 0;
																			         break;
																			 case 2: 
																					if(Dn >= 2)
																					{
																				       Dn -= 2;
																					}
																							 myOLED_String(2,121,"-");
																							 Menu->add_sub = 0;
																			         break;
																		 }
															     break;
													 }
												 break;
								case 6:  //第二层	调头 速度									 												 
								        myOLED_String(1,40,"*");
										switch(Menu->choice_flag % 10)
										{
											case 0:  // 第三层
												break;
											case 1:  //
												{
													 myOLED_String(1,77,"*");
													 switch(Menu->add_sub)
													 {
													   case 0:
																break;
													   case 1: 
																Ren += 2;
																
																myOLED_String(1,121,"+");
																 
																Menu->add_sub = 0;
																break;
														 case 2: 
																if(Ren >= 2)
																{
																	Ren -= 2;
																}
															   
																 myOLED_String(1,121,"-");
																 Menu->add_sub = 0;
																 break;
														 default:
																break;
													 }
													 break;
												}
											default:
												break;
										 }
										 break;
								default:
									break;
							}
					}
					break;  //end of sudu1
			case 2:   //Sudu2		
					{					
				      if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}			        
						myOLED_String(4,1,"*");
						myOLED_String(6,10,"SuDu1");myOLED_String(6,50,"Agle");myOLED_Dec(6,87,Agle);
						myOLED_String(4,10,"SuDu2");myOLED_String(5,50,"LuZ");myOLED_Dec(5,87,LuZ);
						myOLED_String(2,10,"YaoTou");myOLED_String(4,50,"Add3");myOLED_Dec(4,87,Add3);
													myOLED_String(3,50,"S_K");myOLED_Dec(3,87,S_K);
													myOLED_String(2,50,"E_K");myOLED_Dec(2,87,E_K);
													myOLED_String(1,50,"D_K");myOLED_Dec(1,87,D_K);
					  
						switch(Menu->choice_flag % 100 / 10)
						{
							
							case 1:  ////第二层	Agle
									myOLED_String(6,40,"*");
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //					
														 myOLED_String(6,77,"*");	
														 switch(Menu->add_sub)
														 {
															 case 0:
																			 break;
															 case 1: 
																			 Agle += 2;
																			 myOLED_String(6,121,"+");																			         
																			 Menu->add_sub = 0;
																			 break;
															 case 2: 
																			 
																			if(Agle >= 2)
																			{
																				Agle -= 2;
																			}
																			
																			 myOLED_String(6,121,"-");
																			 Menu->add_sub = 0;
																			 break;
														 }
													 break;
									 }
								 break;
							case 2: //第二层	路障	速度	
									{									 
										 myOLED_String(5,40,"*");										 
										 switch(Menu->choice_flag % 10)
										{
											case 0:  // 第三层
												break;
											case 1:  //		
											{															
												 myOLED_String(5,77,"*");	
												 switch(Menu->add_sub)
												 {
												   case 0:
															break;
												   case 1: 
															LuZ += 2;
															myOLED_String(5,121,"+");																			         
															Menu->add_sub = 0;
															break;
													 case 2: 
															if(LuZ >= 2)
															{
															   LuZ -= 2;
															}
															 myOLED_String(5,121,"-");
															 Menu->add_sub = 0;
															 break;
													 default:
														 break;
												 }
											 }
											 break;
										 default:
											 break;
										 }
									}
									break;
							case 3: //第二层	Add3
									{									 
										 myOLED_String(4,40,"*");										 
										 switch(Menu->choice_flag % 10)
										{
											case 0:  // 第三层
												break;
											case 1:  //		
											{															
												 myOLED_String(4,77,"*");	
												 switch(Menu->add_sub)
												 {
												   case 0:
															break;
												   case 1: 
															if(Add3 < 50)
															{
																Add3 += 2;
															}
															myOLED_String(4,121,"+");																			         
															Menu->add_sub = 0;
															break;
													 case 2: 
															if(Add3 >= 2)
															{
															   Add3 -= 2;
															}
															 myOLED_String(4,121,"-");
															 Menu->add_sub = 0;
															 break;
													 default:
														 break;
												 }
											 }
											 break;
										 default:
											 break;
										 }
									}
									break;
							case 4:  ////第二层	S_K
									myOLED_String(3,40,"*");
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //					
														 myOLED_String(3,77,"*");	
														 switch(Menu->add_sub)
														 {
															 case 0:
																			 break;
															 case 1: 
																			 S_K += 5;
																			 myOLED_String(3,121,"+");																			         
																			 Menu->add_sub = 0;
																			 break;
															 case 2: 
																			 
																			if(S_K >= 5)
																			{
																				S_K -= 5;
																			}
																			
																			 myOLED_String(3,121,"-");
																			 Menu->add_sub = 0;
																			 break;
														 }
													 break;
									 }
								 break;
							case 5:  ////第二层	E_K
									myOLED_String(2,40,"*");
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //					
														 myOLED_String(2,77,"*");	
														 switch(Menu->add_sub)
														 {
															 case 0:
																			 break;
															 case 1: 
																			 E_K += 1;
																			 myOLED_String(2,121,"+");																			         
																			 Menu->add_sub = 0;
																			 break;
															 case 2: 
																			 
																			if(E_K >= 1)
																			{
																				E_K -= 1;
																			}
																			
																			 myOLED_String(2,121,"-");
																			 Menu->add_sub = 0;
																			 break;
														 }
													 break;
									 }
								 break;
							case 6:  ////第二层	D_K
									myOLED_String(1,40,"*");
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //					
											 myOLED_String(1,77,"*");	
											 switch(Menu->add_sub)
											 {
												 case 0:
														break;
												case 1: 
														 D_K += 10;
														 myOLED_String(1,121,"+");																			         
														 Menu->add_sub = 0;
														 break;
												 case 2: 
																 
														if(D_K >= 10)
														{
															D_K -= 10;
														}
														
														 myOLED_String(1,121,"-");
														 Menu->add_sub = 0;
														 break;
											 }
											break;
									 default:
										 break;
									 }
								 break;
							default:
									 break;
							}
					}
					break;
			case 3:	//YaoTou
					{
						if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}
						
						myOLED_String(4,1,"*");
						myOLED_String(6,10,"SuDu2");myOLED_String(6,50,"PH");myOLED_Dec(6,87,PH);
													myOLED_String(5,50,"IH");myOLED_Decimals(5,87,IH);											
						myOLED_String(4,10,"YaoTou");
						myOLED_String(2,10,"Duoji");
						
						switch(Menu->choice_flag % 100 / 10)
						{
							case 1:  //第二层	PH		
								{							
									myOLED_String(6,40,"*"); 
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //
											{
												 myOLED_String(6,77,"*");
												 switch(Menu->add_sub)
												 {
												   case 0:
														break;
													 case 1: 
														{
															PH += 5;
															myOLED_String(6,121,"+");
															 
															Menu->add_sub = 0;
														}
														break;
													 case 2: 
														{
															if(PH >= 5)
															{
																PH -= 5;
															}
															myOLED_String(6,121,"-");
															Menu->add_sub = 0;
														}
														break;
													 default:
														 break;
												 }
											}
											break;
										 default:
											 break;
									 }
								}
								break;
							case 2:  //第二层	IH		
								{							
									myOLED_String(5,40,"*"); 
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //
											{
												 myOLED_String(5,77,"*");
												 switch(Menu->add_sub)
												 {
												   case 0:
														break;
													 case 1: 
														{
															IH += 0.1;
															myOLED_String(5,121,"+");
															 
															Menu->add_sub = 0;
														}
														break;
													 case 2: 
														{
															if(IH >= 0.1)
															{
																IH -= 0.1;
															}
															
															myOLED_String(5,121,"-");
															Menu->add_sub = 0;
														}
														break;
													 default:
														 break;
												 }
											}
											break;
									 default:
										 break;
									 }
								}
								break;
							default:
								break;
						}
					}
					break;//end of YaoTou
			case 4:  //Duoji
					{
							if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}
							myOLED_String(4,1,"*");
							myOLED_String(6,10,"YaoTou");myOLED_String(6,50,"P1");myOLED_Dec(6,87,P1);
														 myOLED_String(5,50,"P2");myOLED_Dec(5,87,P2);											
							myOLED_String(4,10,"Duoji"); myOLED_String(4,50,"P3");myOLED_Dec(4,87,P3);
														 myOLED_String(3,50,"H1");myOLED_Decimals(3,87,H1);
							myOLED_String(2,10,"--V--"); myOLED_String(2,50,"H2");myOLED_Decimals(2,87,H2);
														 myOLED_String(1,50,"H3");myOLED_Decimals(1,87,H3);
							
							switch(Menu->choice_flag % 100 / 10)
							{
								 case 1: //第二层	P1									 											 
												 myOLED_String(6,40,"*");
												 switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //					
								                     myOLED_String(6,77,"*");	
													 switch(Menu->add_sub)
													 {
													   case 0:
																   break;
														 case 1: 
																   P1 += 5;
																 myOLED_String(6,121,"+");																			         
																		 Menu->add_sub = 0;
																 break;
														 case 2: 
																if(P1 >= 5)
																{
																   P1 -= 5;
																}
																 myOLED_String(6,121,"-");
																 Menu->add_sub = 0;
																 break;
													 }
													break;
													 }
												 break;
									case 2:  ////第二层	P2
												  myOLED_String(5,40,"*");
												  switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //					
								                     myOLED_String(5,77,"*");	
																		 switch(Menu->add_sub)
																		 {
																		     case 0:
																				       break;
																			 case 1: 
																				       P2 += 1;
																			         myOLED_String(5,121,"+");																			         
																							 Menu->add_sub = 0;
																			         break;
																			 case 2: 
																						if(P2 >= 1)
																						{
																							P2 -= 1;
																						}
																						myOLED_String(5,121,"-");
																					    Menu->add_sub = 0;
																			         break;
																		 }
															     break;
													 }
												 break;
									case 3:  ////第二层	P3
										myOLED_String(4,40,"*");
										switch(Menu->choice_flag % 10)
										{
											case 0:  // 第三层
												break;
											case 1:  //					
													 myOLED_String(4,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		 P3 += 1;
																		 myOLED_String(4,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		if(P3 >= 1)
																		{
																			P3 -= 1;
																		}
																		 myOLED_String(4,121,"-");
																		 Menu->add_sub = 0;
																		 break;
														 default:
															 break;
													 }
													break;
															 
											 default:
												 break;
										 }
										 break;
									 case 4://H1
									 {
										myOLED_String(3,40,"*");
										switch(Menu->choice_flag % 10)
										{
											case 0:  // 第三层
												break;
											case 1:  //					
													 myOLED_String(3,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		 H1 += 0.1;
																		 myOLED_String(3,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		if(H1 >= 0.1)
																		{
																			H1 -= 0.1;
																		}
																		 myOLED_String(3,121,"-");
																		 Menu->add_sub = 0;
																		 break;
														 default:
															 break;
													 }
													break;
															 
											 default:
												 break;
										 }
									 }	
									 break;
									 case 5://H2
									 {
										myOLED_String(2,40,"*");
										switch(Menu->choice_flag % 10)
										{
											case 0:  // 第三层
												break;
											case 1:  //					
													 myOLED_String(2,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		 H2 += 0.05;
																		 myOLED_String(2,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		if(H2 >= 0.05)
																		{
																			H2 -= 0.05;
																		}
																		 myOLED_String(2,121,"-");
																		 Menu->add_sub = 0;
																		 break;
														 default:
															 break;
													 }
													break;
															 
											 default:
												 break;
										 }
									 }	
									 break;
									 case 6://H3
									 {
										myOLED_String(1,40,"*");
										switch(Menu->choice_flag % 10)
										{
											case 0:  // 第三层
												break;
											case 1:  //					
													 myOLED_String(1,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		 H3 += 0.05;
																		 myOLED_String(1,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		if(H3 >= 0.05)
																		{
																			H3 -= 0.05;
																		}
																		 myOLED_String(1,121,"-");
																		 Menu->add_sub = 0;
																		 break;
														 default:
															 break;
													 }
													break;
															 
											 default:
												 break;
										 }
									 }	
									 break;
								    default:
									    break;
								
							}
					}
					break;  // end of duoji
			case 5:   //--V--
					{
				      if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}			        
							myOLED_String(4,1,"*");
							myOLED_String(6,10,"Duoji");myOLED_String(6,50,"VPT");myOLED_Dec(6,87,VPT);
							myOLED_String(4,10,"--V--");myOLED_String(4,50,"VP");myOLED_Dec(4,87,VP);
							myOLED_String(2,10,"Time");myOLED_String(3,50,"VI");myOLED_Dec(3,87,VI);
													   myOLED_String(2,50,"VD");myOLED_Dec(2,87,VD);
														
							switch(Menu->choice_flag % 100 / 10)
							{
								case 1:  //第二层	VPT										 												 
								          myOLED_String(6,40,"*");
													switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //
																		 myOLED_String(6,77,"*");
																		 switch(Menu->add_sub)
																		 {
																		   case 0:
																				       break;
																			 case 1: 
																				       VPT += 20;
																			         myOLED_String(6,121,"+");
																			         
																							 Menu->add_sub = 0;
																			         break;
																			 case 2: 
																					if(VPT >= 20)
																					{
																				       VPT -= 20;
																					}
																							 myOLED_String(6,121,"-");
																							 Menu->add_sub = 0;
																			         break;
																		 }
															     break;
													 }
												 break;
								case 2:  //第二层	VP										 												 
								          myOLED_String(4,40,"*");
													switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //
																		 myOLED_String(4,77,"*");
																		 switch(Menu->add_sub)
																		 {
																		   case 0:
																				       break;
																			 case 1: 
																				       VP += 20;
																			         myOLED_String(4,121,"+");
																			         
																							 Menu->add_sub = 0;
																			         break;
																			 case 2: 
																					if(VP >= 20)
																					{
																				       VP -= 20;
																					}
																							 myOLED_String(4,121,"-");
																							 Menu->add_sub = 0;
																			         break;
																		 }
															     break;
													 }
												 break;
								 case 3: //第二层	VI								 											  
												 myOLED_String(3,40,"*");										 
												 switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //					
								                     myOLED_String(3,77,"*");	
																		 switch(Menu->add_sub)
																		 {
																		   case 0:
																				       break;
																			 case 1: 
																				       VI += 5;
																			         myOLED_String(3,121,"+");																			         
																							 Menu->add_sub = 0;
																			         break;
																			 case 2: 
																					if(VI >= 5)
																					{
																				       VI -= 5;
																					}
																							 myOLED_String(3,121,"-");
																							 Menu->add_sub = 0;
																			         break;
																		 }
															     break;
													 }
													break;
									case 4: //第二层	VD								 											  
												 myOLED_String(2,40,"*");										 
												 switch(Menu->choice_flag % 10)
													{
														case 0:  // 第三层
															break;
														case 1:  //					
								                     myOLED_String(2,77,"*");	
																		 switch(Menu->add_sub)
																		 {
																		   case 0:
																				       break;
																			 case 1: 
																				       VD += 1;
																			         myOLED_String(2,121,"+");																			         
																							 Menu->add_sub = 0;
																			         break;
																			 case 2: 
																						if(VD >= 1)
																						{
																							VD -= 1;
																						}
																							 myOLED_String(2,121,"-");
																							 Menu->add_sub = 0;
																			         break;
																		 }
															     break;
													 }
								default:
									{
									}
									break;
							}
					}
					break;
			case 6:	  // Time
					{
						if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}
						
						myOLED_String(4,1,"*");
						myOLED_String(6,10,"--V--");myOLED_String(6,50,"D-T");myOLED_Dec(6,87,D_T);
													myOLED_String(5,50,"Po"); myOLED_Dec(5,87,Po_T);											
						myOLED_String(4,10,"Time"); myOLED_String(4,50,"Ren");myOLED_Dec(4,87,Ren_T);
						myOLED_String(2,10,"juli"); myOLED_String(3,50,"LuZ");myOLED_Dec(3,87,LuZ_T);
													myOLED_String(2,50,"S_T");myOLED_Dec(2,87,S_T);
						
						switch(Menu->choice_flag % 100 / 10)
						{
							case 1:  //第二层	D_T	
								{							
									myOLED_String(6,40,"*"); 
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //
											{
												 myOLED_String(6,77,"*");
												 switch(Menu->add_sub)
												 {
												   case 0:
														break;
													 case 1: 
														{
															D_T += 1;
															myOLED_String(6,121,"+");
															 
															Menu->add_sub = 0;
														}
														break;
													 case 2: 
														{
															if(D_T >= 1)
															{
																D_T -= 1;
															}
															myOLED_String(6,121,"-");
															Menu->add_sub = 0;
														}
														break;
													 default:
														 break;
												 }
											}
											break;
										 default:
											 break;
									 }
								}
								break;
							case 2:  //第二层	Po_T	
								{							
									myOLED_String(5,40,"*"); 
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //
											{
												 myOLED_String(5,77,"*");
												 switch(Menu->add_sub)
												 {
												   case 0:
														break;
													 case 1: 
														{
															Po_T += 1;
															myOLED_String(5,121,"+");
															 
															Menu->add_sub = 0;
														}
														break;
													 case 2: 
														{
															if(Po_T >= 1)
															{
																Po_T -= 1;
															}
															
															myOLED_String(5,121,"-");
															Menu->add_sub = 0;
														}
														break;
													 default:
														 break;
												 }
											}
											break;
									 default:
										 break;
									 }
								}
								break;
							case 3:  //第二层	Ren_T	
								{							
									myOLED_String(4,40,"*"); 
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //
											{
												 myOLED_String(4,77,"*");
												 switch(Menu->add_sub)
												 {
												   case 0:
														break;
													 case 1: 
														{
															Ren_T += 1;
															myOLED_String(4,121,"+");
															 
															Menu->add_sub = 0;
														}
														break;
													 case 2: 
														{
															if(Ren_T >= 1)
															{
																Ren_T -= 1;
															}
															
															myOLED_String(4,121,"-");
															Menu->add_sub = 0;
														}
														break;
													 default:
														 break;
												 }
											}
											break;
									 default:
										 break;
									 }
								}
								break;
							case 4:  //第二层	LuZ_T	
								{							
									myOLED_String(3,40,"*"); 
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //
											{
												 myOLED_String(3,77,"*");
												 switch(Menu->add_sub)
												 {
												   case 0:
														break;
													 case 1: 
														{
															LuZ_T += 1;
															myOLED_String(3,121,"+");
															 
															Menu->add_sub = 0;
														}
														break;
													 case 2: 
														{
															if(LuZ_T >= 1)
															{
																LuZ_T -= 1;
															}
															
															myOLED_String(3,121,"-");
															Menu->add_sub = 0;
														}
														break;
													 default:
														 break;
												 }
											}
											break;
									 default:
										 break;
									 }
								}
								break;
							case 5:  //第二层	S_T	
								{							
									myOLED_String(2,40,"*"); 
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //
											{
												 myOLED_String(2,77,"*");
												 switch(Menu->add_sub)
												 {
												   case 0:
														break;
													 case 1: 
														{
															S_T += 1;
															myOLED_String(2,121,"+");
															 
															Menu->add_sub = 0;
														}
														break;
													 case 2: 
														{
															if(S_T >= 1)
															{
																S_T -= 1;
															}
															
															myOLED_String(2,121,"-");
															Menu->add_sub = 0;
														}
														break;
													 default:
														 break;
												 }
											}
											break;
									 default:
										 break;
									 }
								}
								break;
							default:
								break;
						}
					}
					break;// end of Time
			case 7:   // juli
					{
				      if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}			        

						myOLED_String(4,1,"*"); 
						myOLED_String(6,10,"Time"); myOLED_String(6,50,"jl_3"); myOLED_Dec(6,87,jl_3);
						myOLED_String(4,10,"juli"); myOLED_String(5,50,"R-l"); myOLED_Dec(5,87,R_l);
						myOLED_String(2,10,"Other");myOLED_String(4,50,"R-L"); myOLED_Dec(4,87,R_L);
													myOLED_String(3,50,"Po_S1");myOLED_Dec(3,87,Po_S1);
													myOLED_String(2,50,"Po_S2");myOLED_Dec(2,87,Po_S2);
													myOLED_String(1,50,"Po_X");myOLED_Dec(1,87,Po_X);
													
						switch(Menu->choice_flag % 100 / 10)
						{
							case 1:  //第二层	jl-3									 												 
									  myOLED_String(6,40,"*");
												switch(Menu->choice_flag % 10)
												{
													case 0:  // 第三层
														break;
													case 1:  //
																	 myOLED_String(6,77,"*");
																	 switch(Menu->add_sub)
																	 {
																	   case 0:
																				   break;
																		 case 1: 
																				if(jl_3 < 1200)
																				{
																				   jl_3 += 50;
																				}
																				myOLED_String(6,121,"+");																			         
																				Menu->add_sub = 0;
																				break;
																		 case 2: 
																				   if(jl_3 >= 50)
																					{
																						jl_3 -= 50;
																					}
																						 myOLED_String(6,121,"-");
																						 Menu->add_sub = 0;
																				 break;
																		 default:
																			 break;
																	 }
															 break;
												 }
											 break;
								case 2: //第二层	L_2									 												 
									  myOLED_String(5,40,"*");
												switch(Menu->choice_flag % 10)
												{
													case 0:  // 第三层
														break;
													case 1:  //
																	 myOLED_String(5,77,"*");
																	 switch(Menu->add_sub)
																	 {
																	   case 0:
																				   break;
																		 case 1: 
																				if(R_l < 50)
																				{
																				   R_l+= 5;
																				}
																				 myOLED_String(5,121,"+");																			         
																						 Menu->add_sub = 0;
																				 break;
																		 case 2: 
																				   if(R_l >= 5)
																				  {
																					R_l -= 5;
																				  }
																						 myOLED_String(5,121,"-");
																						 Menu->add_sub = 0;
																				 break;
																		 default:
																			 break;
																	 }
															 break;
												 }
											 break;
								case 3: //R_L							 												 
									  myOLED_String(4,40,"*");
										switch(Menu->choice_flag % 10)
										{
											case 0:  // 第三层
												break;
											case 1:  //
												 myOLED_String(4,77,"*");
												 switch(Menu->add_sub)
												 {
												   case 0:
															   break;
													 case 1: 
															if(R_L < 50)
															{
															   R_L += 5;
															}
															 myOLED_String(4,121,"+");																			         
																	 Menu->add_sub = 0;
															 break;
													 case 2: 
															   if(R_L >= 5)
																{
																	R_L -= 5;
																}						
																myOLED_String(4,121,"-");
																Menu->add_sub = 0;
															 break;
													 default:
														 break;
												 }
												break;
										 }
										break;
								case 4: //Po_S1							 												 
									  myOLED_String(3,40,"*");
												switch(Menu->choice_flag % 10)
												{
													case 0:  // 第三层
														break;
													case 1:  //
																	 myOLED_String(3,77,"*");
																	 switch(Menu->add_sub)
																	 {
																	   case 0:
																				   break;
																		 case 1: 
																				if(Po_S1 < 300)
																				{
																				   Po_S1 += 5;
																				}
																				 myOLED_String(3,121,"+");																			         
																						 Menu->add_sub = 0;
																				 break;
																		 case 2: 
																				   if(Po_S1 >= 5)
																				  {
																					Po_S1 -= 5;
																				  }
																						 myOLED_String(3,121,"-");
																						 Menu->add_sub = 0;
																				 break;
																		 default:
																			 break;
																	 }
															 break;
												 }
											 break;
								case 5: //Po_S2								 												 
									  myOLED_String(2,40,"*");
												switch(Menu->choice_flag % 10)
												{
													case 0:  // 第三层
														break;
													case 1:  //
																	 myOLED_String(2,77,"*");
																	 switch(Menu->add_sub)
																	 {
																	   case 0:
																				   break;
																		 case 1: 
																				if(Po_S2 < 300)
																				{
																				   Po_S2 += 5;
																				}
																				 myOLED_String(2,121,"+");																			         
																						 Menu->add_sub = 0;
																				 break;
																		 case 2: 
																				   if(Po_S2 >= 5)
																				  {
																					Po_S2 -= 5;
																				  }
																						 myOLED_String(2,121,"-");
																						 Menu->add_sub = 0;
																				 break;
																		 default:
																			 break;
																	 }
															 break;
												 }
											 break;
								case 6: //Po_X									 												 
									  myOLED_String(1,40,"*");
										switch(Menu->choice_flag % 10)
										{
											case 0:  // 第三层
												break;
											case 1:  //
												 myOLED_String(1,77,"*");
												 switch(Menu->add_sub)
												 {
												   case 0:
															   break;
													 case 1: 
															if(Po_X < 300)
															{
															   Po_X += 5;
															}
															 myOLED_String(1,121,"+");																			         
																	 Menu->add_sub = 0;
															 break;
													 case 2: 
															   if(Po_X >= 5)
																{
																	Po_X -= 5;
																}						
																myOLED_String(1,121,"-");
																Menu->add_sub = 0;
															 break;
													 default:
														 break;
												 }
												break;
										 }
										break;
								 default:
									 break;
						 }
					 }
					 break;	
			case 8:	//Other
					{
						if(Menu->Clear)
						{ 
							myOLED_Clear();
							Menu->Clear=0;
						}
						myOLED_String(4,1,"*");
						myOLED_String(6,10,"juli");	myOLED_String(6,50,"R-tp");myOLED_Dec(6,87,R_tp);
						myOLED_String(4,10,"Other");myOLED_String(5,50,"Ramp");myOLED_Dec(5,87,Ramp);
						myOLED_String(2,10,"Go");	myOLED_String(4,50,"Wu_x");myOLED_Dec(4,87,Wu_x);
													myOLED_String(3,50,"LSBR");myOLED_Dec(3,87,LSBR);
													myOLED_String(2,50,"S_L");myOLED_Dec(2,87,S_L);
													myOLED_String(1,50,"CCDL");myOLED_Dec(1,87,CCDL);
						switch(Menu->choice_flag % 100 / 10)
						{
							case 1: //第二层	R_tp	
								{							
									myOLED_String(6,40,"*");
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //R_tp
											{
												 myOLED_String(6,77,"*");
												 switch(Menu->add_sub)
												 {
													 case 0:
															   break;
													 case 1: 
															if(R_tp < 2)
															{
																R_tp += 1;
															}
															myOLED_String(6,121,"+");																			         
															Menu->add_sub = 0;
															break;
													 case 2: 
															if(R_tp > 0)
															{
																 R_tp -= 1;
															}
															 myOLED_String(6,121,"-");
															 Menu->add_sub = 0;
															 break;
													 default:
															break;
												 }
											 }
											 break;
										 default:
											 break;
									 }
								 }
								 break;
							case 2:  //Ramp
								{
									myOLED_String(5,40,"*");
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //R_tp
											{
												 myOLED_String(5,77,"*");
												 switch(Menu->add_sub)
												 {
													 case 0:
															   break;
													 case 1: 
															if(Ramp < 2000)
															{
																Ramp += 50;
															}
															myOLED_String(5,121,"+");																			         
															Menu->add_sub = 0;
															break;
													 case 2: 
															if(Ramp > 50)
															{
																 Ramp -= 50;
															}
															 myOLED_String(5,121,"-");
															 Menu->add_sub = 0;
															 break;
													 default:
															break;
												 }
											 }
											 break;
										 default:
											 break;
									 }
								 }
								 break;
							case 3:  //Wu_x
								{
									 myOLED_String(4,40,"*");
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //R_tp
											{
												 myOLED_String(4,77,"*");
												 switch(Menu->add_sub)
												 {
													 case 0:
															   break;
													 case 1: 
															if(Wu_x != TRUE)
															{
																Wu_x = TRUE;
															}
															myOLED_String(4,121,"+");																			         
															Menu->add_sub = 0;
															break;
													 case 2: 
															if(Wu_x != FALSE)
															{
																 Wu_x = FALSE;
															}
															 myOLED_String(4,121,"-");
															 Menu->add_sub = 0;
															 break;
													 default:
															break;
												 }
											 }
											 break;
										 default:
											 break;
									 }
								 }
								 break;
							case 4:  //LSBR
								{
									myOLED_String(3,40,"*");
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //
											{
												 myOLED_String(3,77,"*");
												 switch(Menu->add_sub)
												 {
													 case 0:
															   break;
													 case 1: 
															if(0 == LSBR)
															{
																LSBR = 1;
															}
															myOLED_String(3,121,"+");																			         
															Menu->add_sub = 0;
															break;
													 case 2: 
															if(1 == LSBR)
															{
																LSBR = 0;
															}
															 myOLED_String(3,121,"-");
															 Menu->add_sub = 0;
															 break;
													 default:
															break;
												 }
											 }
											 break;
										 default:
											 break;
									 }
								 }
								 break;
							case 5:  //S_L
								{
									myOLED_String(2,40,"*");
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //
											{
												 myOLED_String(2,77,"*");
												 switch(Menu->add_sub)
												 {
													 case 0:
															   break;
													 case 1: 
															if(0 == S_L)
															{
																S_L = 1;
															}
															myOLED_String(2,121,"+");																			         
															Menu->add_sub = 0;
															break;
													 case 2: 
															if(1 == S_L)
															{
																S_L = 0;
															}
															 myOLED_String(2,121,"-");
															 Menu->add_sub = 0;
															 break;
													 default:
															break;
												 }
											 }
											 break;
										 default:
											 break;
									 }
								 }
								 break;
							case 6:  //CCDL
								{
									myOLED_String(1,40,"*");
									switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //
											{
												 myOLED_String(1,77,"*");
												 switch(Menu->add_sub)
												 {
													 case 0:
															   break;
													 case 1: 
															if(0 == CCDL)
															{
																CCDL = 1;
															}
															myOLED_String(1,121,"+");																			         
															Menu->add_sub = 0;
															break;
													 case 2: 
															if(1 == CCDL)
															{
																CCDL = 0;
															}
															 myOLED_String(1,121,"-");
															 Menu->add_sub = 0;
															 break;
													 default:
															break;
												 }
											 }
											 break;
										 default:
											 break;
									 }
								 }
								 break;
							default:
								 break;
						}
					}
					break;
			 case 9:  // Go
					{
						if(Menu->Clear)
							{ 
								myOLED_Clear();
								Menu->Clear=0;
							}
							myOLED_String(4,1,"*");
							myOLED_String(6,10,"Other");										
							myOLED_String(4,10,"Go");
							myOLED_String(2,10,"ChkCCD");					
							myOLED_String(4,50,"Ready--Go");
							switch(Menu->choice_flag % 100 / 10)
							{
								case 1:  //第二层										 												 
										myOLED_String(4,40,"*");
										Menu->Ready_Go =  0;
								
										Car_mode = SelfDef;
								
										break;
							}
					}
					break;
			case 10:  // ChkCCD
					{
						if(Menu->Clear)
						{ 
							myOLED_Clear();
							Menu->Clear=0;
						}
						myOLED_String(4,1,"*");
						myOLED_String(6,10,"Go");										
						myOLED_String(4,10,"ChkCCD");
						myOLED_String(2,10,"ChkSL");	
						myOLED_String(4,50,"CheckCCD");
						
						switch(Menu->choice_flag % 100 / 10)
						{
							case 1:  //第二层	
								{								
									myOLED_String(4,40,"*");
									Menu->Ready_Go =  0;
									
									Car_mode = CheckCCD;
									
								}
								break;
							default:
								break;
						}
					}
					break;
			case 11:  //ChkSL
				{
					if(Menu->Clear)
					{ 
						myOLED_Clear();
						Menu->Clear=0;
					}
					myOLED_String(4,1,"*");
					myOLED_String(6,10,"ChkCCD");										
					myOLED_String(4,10,"ChkSL");
					myOLED_String(2,10,"ChkRp");	
					myOLED_String(4,50,"CheckSL");
					
					switch(Menu->choice_flag % 100 / 10)
					{
						case 1:  //第二层										 												 
								myOLED_String(4,40,"*");
								Menu->Ready_Go =  0;
								
								Car_mode = CheckSL;
								
								break;
						default:
							break;
					}
				}
				break;
			case 12: //ChkRp
				{
					if(Menu->Clear)
					{ 
						myOLED_Clear();
						Menu->Clear=0;
					}
					myOLED_String(4,1,"*");
					myOLED_String(6,10,"ChkSL");										
					myOLED_String(4,10,"ChkRp");
					myOLED_String(2,10,"ShiZ");	
					myOLED_String(4,50,"CheckRp");
					
					switch(Menu->choice_flag % 100 / 10)
					{
						case 1:  //第二层										 												 
								myOLED_String(4,40,"*");
								Menu->Ready_Go =  0;
								
								Car_mode = CheckRamp;
								
								break;
						default:
							break;
					}
				}
				break;
			#ifdef RemRoad_Control_Enable
				case 13://ShiZ
				{
					if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}			        
					myOLED_String(4,1,"*");
					myOLED_String(6,10,"ChkRp");myOLED_String(6,50,"S_Z1");myOLED_Dec(6,87,S_Z1);
					myOLED_String(4,10,"ShiZ");myOLED_String(5,50," SQ1");myOLED_Dec(5,87,SQ1);
					myOLED_String(2,10,"RenZ");myOLED_String(4,50," SG1");myOLED_Dec(4,87,SG1);
												myOLED_String(3,50,"S_Z2");myOLED_Dec(3,87,S_Z2);
												myOLED_String(2,50," SQ2");myOLED_Dec(2,87,SQ2);
												myOLED_String(1,50," SG2");myOLED_Dec(1,87,SG2);
				  
					switch(Menu->choice_flag % 100 / 10)
					{
						
						case 1:  //S_Z1
								myOLED_String(6,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(6,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		if(S_Z1 < 16)
																		{
																			S_Z1 += 1;
																		}
																		 myOLED_String(6,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(S_Z1 > 1)
																		{
																			S_Z1 -= 1;
																		}
																		
																		 myOLED_String(6,121,"-");
																		 Menu->add_sub = 0;
																		 break;
													 }
												 break;
								 }
							 break;
						case 2: //SQ1	
								{									 
									 myOLED_String(5,40,"*");										 
									 switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //		
										{															
											 myOLED_String(5,77,"*");	
											 switch(Menu->add_sub)
											 {
											   case 0:
														break;
											   case 1: 
														if(SQ1 < 800)
														{
															SQ1 += 50;
														}
														myOLED_String(5,121,"+");																			         
														Menu->add_sub = 0;
														break;
												 case 2: 
														if(SQ1 >= 50)
														{
														   SQ1 -= 50;
														}
														 myOLED_String(5,121,"-");
														 Menu->add_sub = 0;
														 break;
												 default:
													 break;
											 }
										 }
										 break;
									 default:
										 break;
									 }
								}
								break;
						case 3: //SG1
								{									 
									 myOLED_String(4,40,"*");										 
									 switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //		
										{															
											 myOLED_String(4,77,"*");	
											 switch(Menu->add_sub)
											 {
											   case 0:
														break;
											   case 1: 
														if(SG1 < 800)
														{
															SG1 += 50;
														}
														
														myOLED_String(4,121,"+");																			         
														Menu->add_sub = 0;
														break;
												 case 2: 
														if(SG1 >= 50)
														{
														   SG1 -= 50;
														}
														 myOLED_String(4,121,"-");
														 Menu->add_sub = 0;
														 break;
												 default:
													 break;
											 }
										 }
										 break;
									 default:
										 break;
									 }
								}
								break;
						case 4:  //S_Z2
								myOLED_String(3,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(3,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		if(S_Z2 < 16)
																		{
																			S_Z2 += 1;
																		}
																		 myOLED_String(3,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(S_Z2 > 1)
																		{
																			S_Z2 -= 1;
																		}
																		
																		 myOLED_String(3,121,"-");
																		 Menu->add_sub = 0;
																		 break;
													 }
												 break;
								 }
							 break;
						case 5:  //SQ2
								myOLED_String(2,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(2,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																	if(SQ2 < 800)
																	{
																		SQ2 += 50;
																	}
																		 myOLED_String(2,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(SQ2 >= 50)
																		{
																			SQ2 -= 50;
																		}
																		
																		 myOLED_String(2,121,"-");
																		 Menu->add_sub = 0;
																		 break;
													 }
												 break;
								 }
							 break;
						case 6:  //SG2
								myOLED_String(1,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
										 myOLED_String(1,77,"*");	
										 switch(Menu->add_sub)
										 {
											 case 0:
													break;
											case 1: 
													if(SG2 < 800)
													{
														SG2 += 50;
													}
													 myOLED_String(1,121,"+");																			         
													 Menu->add_sub = 0;
													 break;
											 case 2: 
															 
													if(SG2 >= 50)
													{
														SG2 -= 50;
													}
													
													 myOLED_String(1,121,"-");
													 Menu->add_sub = 0;
													 break;
										 }
										break;
								 default:
									 break;
								 }
							 break;
						default:
								 break;
					}
				}
				break;
				case 14://RenZ
				{
					if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}			        
					myOLED_String(4,1,"*");
					myOLED_String(6,10,"ShiZ");myOLED_String(6,50,"R_Z1");myOLED_Dec(6,87,R_Z1);
					myOLED_String(4,10,"RenZ");myOLED_String(5,50," RQ1");myOLED_Dec(5,87,RQ1);
					myOLED_String(2,10,"Po_D");myOLED_String(4,50," RG1");myOLED_Dec(4,87,RG1);
												myOLED_String(3,50,"R_Z2");myOLED_Dec(3,87,R_Z2);
												myOLED_String(2,50," RQ2");myOLED_Dec(2,87,RQ2);
												myOLED_String(1,50," RG2");myOLED_Dec(1,87,RG2);
				  
					switch(Menu->choice_flag % 100 / 10)
					{
						
						case 1:  //R_Z1
								myOLED_String(6,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(6,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		if(R_Z1 < 2)
																		{
																			R_Z1 += 1;
																		}
																		 myOLED_String(6,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(R_Z1 > 1)
																		{
																			R_Z1 -= 1;
																		}
																		
																		 myOLED_String(6,121,"-");
																		 Menu->add_sub = 0;
																		 break;
													 }
												 break;
								 }
							 break;
						case 2: //RQ1
								{									 
									 myOLED_String(5,40,"*");										 
									 switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //		
										{															
											 myOLED_String(5,77,"*");	
											 switch(Menu->add_sub)
											 {
											   case 0:
														break;
											   case 1: 
														if(RQ1 < 800)
														{
															RQ1 += 50;
														}
														myOLED_String(5,121,"+");																			         
														Menu->add_sub = 0;
														break;
												 case 2: 
														if(RQ1 >= 50)
														{
														   RQ1 -= 50;
														}
														 myOLED_String(5,121,"-");
														 Menu->add_sub = 0;
														 break;
												 default:
													 break;
											 }
										 }
										 break;
									 default:
										 break;
									 }
								}
								break;
						case 3: //RG1
								{									 
									 myOLED_String(4,40,"*");										 
									 switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //		
										{															
											 myOLED_String(4,77,"*");	
											 switch(Menu->add_sub)
											 {
											   case 0:
														break;
											   case 1: 
														if(RG1 < 800)
														{
															RG1 += 50;
														}
														
														myOLED_String(4,121,"+");																			         
														Menu->add_sub = 0;
														break;
												 case 2: 
														if(RG1 >= 50)
														{
														   RG1 -= 50;
														}
														 myOLED_String(4,121,"-");
														 Menu->add_sub = 0;
														 break;
												 default:
													 break;
											 }
										 }
										 break;
									 default:
										 break;
									 }
								}
								break;
						case 4:  //R_Z2
								myOLED_String(3,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(3,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		if(R_Z2 < 2)
																		{
																			R_Z2 += 1;
																		}
																		 myOLED_String(3,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(R_Z2 > 1)
																		{
																			R_Z2 -= 1;
																		}
																		
																		 myOLED_String(3,121,"-");
																		 Menu->add_sub = 0;
																		 break;
													 }
												 break;
								 }
							 break;
						case 5:  //RQ2
								myOLED_String(2,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(2,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																	if(RQ2 < 800)
																	{
																		RQ2 += 50;
																	}
																		 myOLED_String(2,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(RQ2 >= 50)
																		{
																			RQ2 -= 50;
																		}
																		
																		 myOLED_String(2,121,"-");
																		 Menu->add_sub = 0;
																		 break;
													 }
												 break;
								 }
							 break;
						case 6:  //RG2
								myOLED_String(1,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
										 myOLED_String(1,77,"*");	
										 switch(Menu->add_sub)
										 {
											 case 0:
													break;
											case 1: 
													if(RG2 < 800)
													{
														RG2 += 50;
													}
													 myOLED_String(1,121,"+");																			         
													 Menu->add_sub = 0;
													 break;
											 case 2: 
															 
													if(RG2 >= 50)
													{
														RG2 -= 50;
													}
													
													 myOLED_String(1,121,"-");
													 Menu->add_sub = 0;
													 break;
										 }
										break;
								 default:
									 break;
								 }
							 break;
						default:
								 break;
					}
				}
				break;
				case 15://Po_D
				{
					if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}			        
					myOLED_String(4,1,"*");
					myOLED_String(6,10,"RenZ");myOLED_String(6,50,"P_D1");myOLED_Dec(6,87,P_D1);
					myOLED_String(4,10,"Po_D");myOLED_String(5,50," PQ1");myOLED_Dec(5,87,PQ1);
					myOLED_String(2,10,"TeShu");myOLED_String(4,50," PG1");myOLED_Dec(4,87,PG1);
												myOLED_String(3,50,"P_D2");myOLED_Dec(3,87,P_D2);
												myOLED_String(2,50," PQ2");myOLED_Dec(2,87,PQ2);
												myOLED_String(1,50," PG2");myOLED_Dec(1,87,PG2);
				  
					switch(Menu->choice_flag % 100 / 10)
					{
						
						case 1:  //P_D1
								myOLED_String(6,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(6,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		if(P_D1 < 2)
																		{
																			P_D1 += 1;
																		}
																		 myOLED_String(6,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(P_D1 > 1)
																		{
																			P_D1 -= 1;
																		}
																		
																		 myOLED_String(6,121,"-");
																		 Menu->add_sub = 0;
																		 break;
													 }
												 break;
								 }
							 break;
						case 2: //PQ1	
								{									 
									 myOLED_String(5,40,"*");										 
									 switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //		
										{															
											 myOLED_String(5,77,"*");	
											 switch(Menu->add_sub)
											 {
											   case 0:
														break;
											   case 1: 
														if(PQ1 < 800)
														{
															PQ1 += 50;
														}
														myOLED_String(5,121,"+");																			         
														Menu->add_sub = 0;
														break;
												 case 2: 
														if(PQ1 >= 50)
														{
														   PQ1 -= 50;
														}
														 myOLED_String(5,121,"-");
														 Menu->add_sub = 0;
														 break;
												 default:
													 break;
											 }
										 }
										 break;
									 default:
										 break;
									 }
								}
								break;
						case 3: //PG1
								{									 
									 myOLED_String(4,40,"*");										 
									 switch(Menu->choice_flag % 10)
									{
										case 0:  // 第三层
											break;
										case 1:  //		
										{															
											 myOLED_String(4,77,"*");	
											 switch(Menu->add_sub)
											 {
											   case 0:
														break;
											   case 1: 
														if(PG1 < 800)
														{
															PG1 += 50;
														}
														
														myOLED_String(4,121,"+");																			         
														Menu->add_sub = 0;
														break;
												 case 2: 
														if(PG1 >= 50)
														{
														   PG1 -= 50;
														}
														 myOLED_String(4,121,"-");
														 Menu->add_sub = 0;
														 break;
												 default:
													 break;
											 }
										 }
										 break;
									 default:
										 break;
									 }
								}
								break;
						case 4:  //P_D2
								myOLED_String(3,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(3,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		if(P_D2 < 2)
																		{
																			P_D2 += 1;
																		}
																		 myOLED_String(3,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(P_D2 > 1)
																		{
																			P_D2 -= 1;
																		}
																		
																		 myOLED_String(3,121,"-");
																		 Menu->add_sub = 0;
																		 break;
													 }
												 break;
								 }
							 break;
						case 5:  //PQ2
								myOLED_String(2,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(2,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																	if(PQ2 < 800)
																	{
																		PQ2 += 50;
																	}
																		 myOLED_String(2,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(PQ2 >= 50)
																		{
																			PQ2 -= 50;
																		}
																		
																		 myOLED_String(2,121,"-");
																		 Menu->add_sub = 0;
																		 break;
													 }
												 break;
								 }
							 break;
						case 6:  //PG2
								myOLED_String(1,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
										 myOLED_String(1,77,"*");	
										 switch(Menu->add_sub)
										 {
											 case 0:
													break;
											case 1: 
													if(PG2 < 800)
													{
														PG2 += 50;
													}
													 myOLED_String(1,121,"+");																			         
													 Menu->add_sub = 0;
													 break;
											 case 2: 
															 
													if(PG2 >= 50)
													{
														PG2 -= 50;
													}
													
													 myOLED_String(1,121,"-");
													 Menu->add_sub = 0;
													 break;
										 }
										break;
								 default:
									 break;
								 }
							 break;
						default:
								 break;
					}
				}
				break;
				case 16://TeShu 特殊模块功能设定
				{
					if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}			        
					myOLED_String(4,1,"*");
					myOLED_String(6,10,"Po_D"); myOLED_String(6,50,"Jiyi");myOLED_Dec(6,87,Jiyi);
					myOLED_String(4,10,"TeShu");myOLED_String(5,50,"F_C");myOLED_Dec(5,87,F_C);
					myOLED_String(2,10,"Binay"); myOLED_String(4,50,"C_J");myOLED_Dec(4,87,C_J);
												myOLED_String(3,50,"P_Y");myOLED_Dec(3,87,P_Y);
												myOLED_String(2,50,"RNum");myOLED_Dec(2,87,RNum);
					
					switch(Menu->choice_flag % 100 / 10)
					{
						case 1: //Jiyi
							{
								myOLED_String(6,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(6,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		if(1 != Jiyi)
																		{
																			Jiyi = 1;
																		}
																		 myOLED_String(6,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(0 != Jiyi)
																		{
																			Jiyi = 0;
																		}
																		
																		 myOLED_String(6,121,"-");
																		 Menu->add_sub = 0;
																		 break;
														 default:
															 break;
													 }
												 break;
									 default:
										 break;
								 }
							 
							}
							break;
						case 2: //F_C
							{									 
								 myOLED_String(5,40,"*");										 
								 switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //		
									{															
										 myOLED_String(5,77,"*");	
										 switch(Menu->add_sub)
										 {
										   case 0:
													break;
										   case 1: 
													if(F_C < 100)
													{
														F_C += 2;
													}
													myOLED_String(5,121,"+");																			         
													Menu->add_sub = 0;
													break;
											 case 2: 
													if(F_C >= 2)
													{
													   F_C -= 2;
													}
													 myOLED_String(5,121,"-");
													 Menu->add_sub = 0;
													 break;
											 default:
												 break;
										 }
									 }
									 break;
								 default:
									 break;
								 }
							}
							break;
						case 3: //C_J
							{									 
								 myOLED_String(4,40,"*");										 
								 switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //		
									{															
										 myOLED_String(4,77,"*");	
										 switch(Menu->add_sub)
										 {
										   case 0:
													break;
										   case 1: 
													if(1 != C_J)
													{
														C_J = 1;
													}
													myOLED_String(4,121,"+");																			         
													Menu->add_sub = 0;
													break;
											 case 2: 
													if(0 != C_J)
													{
													   C_J = 0;
													}
													 myOLED_String(4,121,"-");
													 Menu->add_sub = 0;
													 break;
											 default:
												 break;
										 }
									 }
									 break;
								 default:
									 break;
								 }
							}
							break;
						case 4: //P_Y
							{									 
								 myOLED_String(3,40,"*");										 
								 switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //		
									{															
										 myOLED_String(3,77,"*");	
										 switch(Menu->add_sub)
										 {
										   case 0:
													break;
										   case 1: 
													if(P_Y < 5)
													{
														P_Y += 1;
													}
													myOLED_String(3,121,"+");																			         
													Menu->add_sub = 0;
													break;
											 case 2: 
													if(P_Y >= 1)
													{
													   P_Y -= 1;
													}
													 myOLED_String(3,121,"-");
													 Menu->add_sub = 0;
													 break;
											 default:
												 break;
										 }
									 }
									 break;
								 default:
									 break;
								 }
							}
							break;
						case 5: //RNum
							{									 
								 myOLED_String(2,40,"*");										 
								 switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //		
									{															
										 myOLED_String(2,77,"*");	
										 switch(Menu->add_sub)
										 {
										   case 0:
													break;
										   case 1: 
													if(RNum < 3)
													{
														RNum += 1;
													}
													myOLED_String(2,121,"+");																			         
													Menu->add_sub = 0;
													break;
											 case 2: 
													if(RNum > 1)
													{
													   RNum -= 1;
													}
													 myOLED_String(2,121,"-");
													 Menu->add_sub = 0;
													 break;
											 default:
												 break;
										 }
									 }
									 break;
								 default:
									 break;
								 }
							}
							break;
						default:
							break;
					}
				}
				break;
			#endif
			case 17://Binay  CCD二值化设定
				{
					if(Menu->Clear){ myOLED_Clear();Menu->Clear=0;}			        
					myOLED_String(4,1,"*");
					myOLED_String(6,10,"TeShu"); myOLED_String(6,50,"B_T");myOLED_Dec(6,87,B_T);
					myOLED_String(4,10,"Binay");myOLED_String(5,50,"b_m");myOLED_Dec(5,87,b_m);
					myOLED_String(2,10,"NULL"); myOLED_String(4,50,"B_M");myOLED_Dec(4,87,B_M);
												myOLED_String(3,50,"Sght");myOLED_Dec(3,87,Sght);
					
					switch(Menu->choice_flag % 100 / 10)
					{
						case 1: //B_T
							{
								myOLED_String(6,40,"*");
								switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //					
													 myOLED_String(6,77,"*");	
													 switch(Menu->add_sub)
													 {
														 case 0:
																		 break;
														 case 1: 
																		if(B_T < 3000)
																		{
																			B_T += 100;
																		}
																		 myOLED_String(6,121,"+");																			         
																		 Menu->add_sub = 0;
																		 break;
														 case 2: 
																		 
																		if(B_T > 800)
																		{
																			B_T -= 100;
																		}
																		
																		 myOLED_String(6,121,"-");
																		 Menu->add_sub = 0;
																		 break;
														 default:
															 break;
													 }
												 break;
									 default:
										 break;
								 }
							 
							}
							break;
						case 2: //b_m
							{									 
								 myOLED_String(5,40,"*");										 
								 switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //		
									{															
										 myOLED_String(5,77,"*");	
										 switch(Menu->add_sub)
										 {
										   case 0:
													break;
										   case 1: 
													if(b_m < 1600)
													{
														b_m += 100;
													}
													myOLED_String(5,121,"+");																			         
													Menu->add_sub = 0;
													break;
											 case 2: 
													if(b_m > 400)
													{
														b_m -= 100;
													}
													 myOLED_String(5,121,"-");
													 Menu->add_sub = 0;
													 break;
											 default:
												 break;
										 }
									 }
									 break;
								 default:
									 break;
								 }
							}
							break;
						case 3: //B_M
							{									 
								 myOLED_String(4,40,"*");										 
								 switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //		
									{															
										 myOLED_String(4,77,"*");	
										 switch(Menu->add_sub)
										 {
										   case 0:
													break;
										   case 1: 
													if(B_M < 2000)
													{
														B_M += 100;
													}
													myOLED_String(4,121,"+");																			         
													Menu->add_sub = 0;
													break;
											 case 2: 
													if(B_M > 600)
													{
														B_M -= 100;
													}
													 myOLED_String(4,121,"-");
													 Menu->add_sub = 0;
													 break;
											 default:
												 break;
										 }
									 }
									 break;
								 default:
									 break;
								 }
							}
							break;
						case 4: //Sght
							{									 
								 myOLED_String(3,40,"*");										 
								 switch(Menu->choice_flag % 10)
								{
									case 0:  // 第三层
										break;
									case 1:  //		
									{															
										 myOLED_String(3,77,"*");	
										 switch(Menu->add_sub)
										 {
										   case 0:
													break;
										   case 1: 
													if(Sght < 600)
													{
														Sght += 50;
													}
													myOLED_String(3,121,"+");																			         
													Menu->add_sub = 0;
													break;
											 case 2: 
													if(Sght >= 50)
													{
														Sght -= 50;
													}
													 myOLED_String(3,121,"-");
													 Menu->add_sub = 0;
													 break;
											 default:
												 break;
										 }
									 }
									 break;
								 default:
									 break;
								 }
							}
							break;
						default:
							break;
					}
				}
				break;
			default :
				{
					Menu->choice_flag -= 100;
				}
				break;
		}

		return Menu->Ready_Go;
		
	}
	
	
	
	
	
