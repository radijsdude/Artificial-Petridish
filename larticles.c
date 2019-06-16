#ifndef LARTICLES_H_INCLUDED
#include "larticles.h"
#endif
void Larticles_Initiate(Larticles *larticles)
{
	Larticle meteor;
	meteor.state = 5;
	meteor.r = METEOR_SIZE;
	meteor.m = METEOR_MASS;
	meteor.state = 10;
	larticles->meteor = meteor;
	larticles->larticles_amount = -1;
	for (int i = 0; i < LARTICLES_START_AMOUNT; i++)
	{
		Larticle p;
		p.r = LARTICLE_SIZE;
		p.m = 5.0f;
		
		float rra = (float)(rand()%100)/100.0f * 2.0f * M_PI;
		float rrd = (float)rand()*UNIVERSE_SIZE/(RAND_MAX)/2;
		p.x = (float)rrd* cos(rra) + (float)UNIVERSE_SIZE/2.0f;
		p.y = (float)rrd* sin(rra) + (float)UNIVERSE_SIZE/2.0f;
		p.vx = 0.0f;
		p.vy = 0.0f;
		p.ax = 0.0f;
		p.ay = 0.0f;
		p.color[0] = 0;
		p.color[1] = 0;
		p.color[2] = 0;
		p.color[3] = 255;
		p.angle = (float)(rand()%200 - 100) / 100.0f * M_PI;
		p.anglespeed = 0.0f;
		p.time = LARTICLE_INITIATE_TIME;
		p.state = 0;

		p.health = LARTICLE_SPLIT_HEALTH;
		p.alive = 1;

		p.connections_length = -1;
		for (int i=0; i<NEURONS_AMOUNT; i++)
		{
			p.potentials[i] = NEURONS_START_POTENTIAL;
			p.errors[i] = 0.0f;
		}
		for (int i=0; i<NEURONS_CONNECTIONS; i++)
		{
			p.connections[i][0] = -13.0f;
			p.connections[i][1] = -13.0f;
			p.weights[i] = 0.0f;
		}
		Larticle_Create_Connections(&p);
		Larticle_Add_Connection(&p, NEURON_ALIVE_1, NEURON_SPLIT,2.0f);
		//Larticle_Add_Connection(&p, NEURON_ALIVE, NEURON_MOVE_X_1,2.0f);
		Larticles_Add(larticles, p);
		
	}
	Larticle_Calculate_All(&larticles->larticles[0]);
	Larticle_Print(&larticles->larticles[0]);
}


void Larticles_Add(Larticles *larticles, Larticle larticle)
{
		if (larticles->larticles_amount < LARTICLES_MAX -1)
		{
			larticles->larticles_amount += 1;
			larticles->larticles[larticles->larticles_amount] = larticle;
		}
}

void Larticles_Remove(Larticles *larticles, int i)
{
	larticles->larticles[i] = larticles->larticles[larticles->larticles_amount];
	larticles->larticles_amount -= 1;
}


void Larticles_Doe(Larticles *larticles)
{
	float lx = 0.0f;
	float ly = 0.0f;
	float la = 0.0f;
	float coss = 0.0f;
	float sinn = 0.0f;
	float mx = 0.0f;
	float my = 0.0f;
	float see_d = 0.0f;
	float see_a = 0.0f;
	int insight = 0;
	float ma1 = 0.0f;
	float ma2 = 0.0f;
	float distance = (float)LARTICLE_VISUAL_RANGE;
	float cx = 0.0f;
	float cy = 0.0f;
	float smax = 0.0f;
	if (larticles->meteor.state < 10)
	{
		Larticle_Gravitate(&larticles->meteor);
		larticles->meteor.vx += larticles->meteor.ax;
		larticles->meteor.vy += larticles->meteor.ay;
		float dv = larticles->meteor.vx*larticles->meteor.vx +
			larticles->meteor.vy*larticles->meteor.vy;

			if (dv > (float)METEOR_MAX_SPEED*(float)METEOR_MAX_SPEED)
			{
				float sdv = sqrt(dv);
				larticles->meteor.vx = (float)METEOR_MAX_SPEED * larticles->meteor.vx / sdv;
				larticles->meteor.vy = (float)METEOR_MAX_SPEED * larticles->meteor.vy / sdv;
			}
		larticles->meteor.x += larticles->meteor.vx;
		larticles->meteor.y += larticles->meteor.vy;
	}
	else
	{
		larticles->meteor.r = (float)METEOR_SIZE;
		float rra = (float)(rand()%100)/100.0f * 2.0f * M_PI;
		larticles->meteor.x = (float)UNIVERSE_SIZE * cos(rra) * 2.0f + (float)UNIVERSE_SIZE/2.0f;
		larticles->meteor.y = (float)UNIVERSE_SIZE * sin(rra) * 2.0f + (float)UNIVERSE_SIZE/2.0f;
		larticles->meteor.vx = (float)(rand()%10 - 5);
		larticles->meteor.vy = (float)(rand()%10 - 5);
		larticles->meteor.state = 7;
	}
	//Larticle_Print(&larticles->meteor);
	for (int i=0; i<larticles->larticles_amount +1; i++)
	{
		if(larticles->larticles[i].health > 0.0f)
		{
			larticles->larticles[i].time += 1;
			
			Larticle_Gravitate(&larticles->larticles[i]);
			


			larticles->larticles[i].vx += larticles->larticles[i].ax;
			larticles->larticles[i].vy += larticles->larticles[i].ay;
			la = larticles->larticles[i].angle;
			sinn = sin(la);
			coss = cos(la);
			lx = larticles->larticles[i].x;
			ly = larticles->larticles[i].y;
			
			if ((larticles->meteor.x - lx) * (larticles->meteor.x - lx) + 
			(larticles->meteor.y - ly) * (larticles->meteor.y - ly) < 
			(larticles->meteor.r + larticles->larticles[i].r) * (larticles->meteor.r + larticles->larticles[i].r))
			{
				Larticle_Collide(&larticles->larticles[i],&larticles->meteor);
				larticles->larticles[i].health = -13.0f;
				larticles->meteor.state += 1;
				larticles->meteor.r -= (float)METEOR_SIZE/10.0f;
			}
			
			if (larticles->larticles[i].time > LARTICLE_TIME)
			{
				
				larticles->larticles[i].r = LARTICLE_SIZE + larticles->larticles[i].health / LARTICLE_MAX_HEALTH *
				 LARTICLE_SIZE_SCALE;
				larticles->larticles[i].m = larticles->larticles[i].r;
				larticles->larticles[i].potentials[NEURON_ALIVE_1] = 1.0f;
				larticles->larticles[i].potentials[NEURON_ALIVE_2] = 1.0f;
				larticles->larticles[i].potentials[NEURON_IN_1] = larticles->larticles[i].potentials[NEURON_OUT_1];
				larticles->larticles[i].potentials[NEURON_IN_2] = larticles->larticles[i].potentials[NEURON_OUT_2];
				larticles->larticles[i].potentials[NEURON_SENSE_ANGLE] = larticles->larticles[i].angle;
				larticles->larticles[i].potentials[NEURON_SENSE_HEALTH] = (float)larticles->larticles[i].health /
				((float)LARTICLE_MAX_HEALTH);
				//larticles->larticles[i].potentials[NEURON_SENSE_ANGLE_SPEED] = larticles->larticles[i].anglespeed;
				larticles->larticles[i].potentials[NEURON_TIME] = sin((float)larticles->larticles[i].time * M_PI / 
				(500.0f * larticles->larticles[i].potentials[NEURON_FREQUENCY]));
				Larticle_Calculate_All(&larticles->larticles[i]);
				
				smax = 0.0f;
				larticles->larticles[i].state = 0;
				if (larticles->larticles[i].potentials[NEURON_STATE_1] > smax)
				{
					larticles->larticles[i].state = 1;
					smax = larticles->larticles[i].potentials[NEURON_STATE_1];
				}
				if (larticles->larticles[i].potentials[NEURON_STATE_2] > smax)
				{
					larticles->larticles[i].state = 2;
					smax = larticles->larticles[i].potentials[NEURON_STATE_2];
				}
				if (larticles->larticles[i].potentials[NEURON_STATE_3] > smax)
				{
					larticles->larticles[i].state = 3;
					smax = larticles->larticles[i].potentials[NEURON_STATE_3];
				}

				if(larticles->larticles[i].state ==1)
				{
					larticles->larticles[i].health += (float)LARTICLES_REGEN;
					larticles->larticles[i].color[0] = 0;
					larticles->larticles[i].color[1] = 0;
					larticles->larticles[i].color[2] = 255;
					if (larticles->larticles[i].potentials[NEURON_ATTACK] > 0.5f)
					{
						larticles->larticles[i].color[1] = 150;
					}
				}
				else
				{
					if(larticles->larticles[i].state == 2)
					{
						larticles->larticles[i].color[0] = 255;
						larticles->larticles[i].color[1] = 0;
						larticles->larticles[i].color[2] = 0;
						if (larticles->larticles[i].potentials[NEURON_ATTACK] > 0.5)
						{
							larticles->larticles[i].color[1] = 150;
						}
					}
					else
					{
						larticles->larticles[i].color[0] = 0;
						larticles->larticles[i].color[1] = 255;
						larticles->larticles[i].color[2] = 0;
						if (larticles->larticles[i].potentials[NEURON_ATTACK] > 0.5)
						{
							larticles->larticles[i].color[0] = 150;
						}
					}
				}

				if (larticles->larticles[i].state != 0)
				{
					larticles->larticles[i].health -= UNIVERSE_DAMAGE * 
					(float)larticles->larticles[i].connections_length/(float)NEURONS_CONNECTIONS;
				}
				if (larticles->larticles[i].state != 1)
				{
					mx = larticles->larticles[i].potentials[NEURON_MOVE_X_1] - 
					larticles->larticles[i].potentials[NEURON_MOVE_X_2];
					my = larticles->larticles[i].potentials[NEURON_MOVE_Y_1] - 
					larticles->larticles[i].potentials[NEURON_MOVE_Y_2];
					
					larticles->larticles[i].vx += (1.0f/((float)LARTICLE_SPEED)*(coss*
					(mx) - sinn*(my)));
					larticles->larticles[i].vy += (1.0f/((float)LARTICLE_SPEED)*(coss*
					(my) + sinn*(mx)));
				}
				cx = lx - (float)UNIVERSE_SIZE / 2.0f;
				cy = ly - (float)UNIVERSE_SIZE / 2.0f;
				

				if (larticles->larticles[i].potentials[NEURON_SPLIT] > 0.5f &&
				larticles->larticles[i].health > (float)LARTICLE_SPLIT_HEALTH*1.1f)
				{
					larticles->larticles[i].health -= (float)LARTICLE_SPLIT_HEALTH;
					Larticle p;
					p.x = lx - 2.1f*(float)LARTICLE_SIZE*coss;
					p.y = ly - 2.1f*(float)LARTICLE_SIZE*sinn;
					p.r = LARTICLE_SIZE;
					p.m = 5.0f;
					p.vx = larticles->larticles[i].vx;
					p.vy = larticles->larticles[i].vy;
					p.ax = 0.0f;
					p.ay = 0.0f;
					p.color[0] = 0;
					p.color[1] = 0;
					p.color[2] = 0;
					p.color[3] = 255;
					p.angle = (float)(rand()%200 - 100) / 100.0f * M_PI;
					p.anglespeed = 0.0f;
					p.time = 0;
					p.state = 0;

					p.health = (float)LARTICLE_SPLIT_HEALTH*0.9f;
					p.alive = 1;

					p.connections_length = -1;
					for (int i=0; i<NEURONS_AMOUNT; i++)
					{
						p.potentials[i] = (float)NEURONS_START_POTENTIAL;
						p.errors[i] = 0.0f;
					}
					for (int i=0; i<NEURONS_CONNECTIONS; i++)
					{
						p.connections[i][0] = -13.0f;
						p.connections[i][1] = -13.0f;
						p.weights[i] = 0.0f;
					}
					Larticle_Heridity(&larticles->larticles[i], &p);
					Larticles_Add(larticles, p);

				}
			}
			else
			{
				larticles->larticles[i].color[0] = 255;
				larticles->larticles[i].color[1] = 255;
				larticles->larticles[i].color[2] = 255;
			}


			//larticles->larticles[i].anglespeed *= (1.0f - 20.0f * UNIVERSE_FRICTION);
			ma1 = 0.0f;
			ma2 = 0.0f;
			if (larticles->larticles[i].potentials[NEURON_MOVE_ANGLE_1] > 0.5f)
			{
				ma1 = larticles->larticles[i].potentials[NEURON_MOVE_ANGLE_1] - 0.5f;
			}
			if (larticles->larticles[i].potentials[NEURON_MOVE_ANGLE_2] > 0.5f)
			{
				ma2 = larticles->larticles[i].potentials[NEURON_MOVE_ANGLE_2] - 0.5f;
			}
			larticles->larticles[i].angle += (ma1 - ma2) * M_PI/12.0f;//larticles->larticles[i].anglespeed;
			if (larticles->larticles[i].angle >= M_PI)
			{
				larticles->larticles[i].angle = -M_PI;
			}
			if (larticles->larticles[i].angle < -M_PI)
			{
				larticles->larticles[i].angle = M_PI;
			}
			
			see_d = 0.0f;
			see_a = 0.0f;
			insight = 0;
			distance = (float)LARTICLE_VISUAL_RANGE;
			Larticle lsee;
			for (int j=0; j<larticles->larticles_amount +1; j++)
			{
				if(larticles->larticles[j].health>0.0f)
				{
					if (i!=j)
					{
						float dx = larticles->larticles[j].x - lx;
						float dy = larticles->larticles[j].y - ly;
						float dr = larticles->larticles[i].r + larticles->larticles[j].r;
						float d = dx*dx + dy*dy;
						if (d==0.0f)
						{
							d = 0.01f;
						}
						if (d < dr*dr)
						{
							Larticle_Collide(&larticles->larticles[i],&larticles->larticles[j]);
						}
						if (d < LARTICLE_VISUAL_RANGE * LARTICLE_VISUAL_RANGE && 
						larticles->larticles[i].time > LARTICLE_TIME)
						{
							float drr = 1.0f/d;
							float daa = (atan2(dy,dx) - la) / d;
							see_d += drr;
							see_a += daa;

							float D =(lx - larticles->larticles[j].x) *
							(ly + sinn - larticles->larticles[j].y) -
							(ly - larticles->larticles[j].y) *
							(lx + coss - larticles->larticles[j].x);
							float rr = larticles->larticles[j].r + tan(LARTICLE_FOV) *  sqrt(d);
							if (D * D < rr * rr)
							{
								float sign = 1;
								if (sinn < 0.0f)
								{
									sign = -1;
								}
								float Dx1 = (D * sinn + sign * coss * sqrt(rr - D*D));
								float Dx2 = (D * sinn - sign * coss * sqrt(rr - D*D));
								float Dy1 = (D * coss + abs(sinn) * sqrt(rr - D*D));
								float Dy2 = ( - D * coss - abs(sinn) * sqrt(rr - D*D));
								int s = 0;
								if (coss >= 0.0f && Dx1 >= 0.0f)
								{
									if (sinn >= 0.0f && Dy1 >=0.0f)
									{
										s = 1;
									}
									if (sinn < 0.0f && Dy1 < 0.0f)
									{
										s = 1;
									}
								}
								if (coss < 0.0f && Dx1 < 0.0f)
								{
									if (sinn >= 0.0f && Dy1 >=0.0f)
									{
										s = 1;
									}
									if (sinn < 0.0f && Dy1 < 0.0f)
									{
										s = 1;
									}
								}
								if (s == 1)
								{
									insight = 1;
									if (distance < d)
									{
										lsee = larticles->larticles[j];
										distance = d;
									}
								}
							}

						}


					}


				}
				else
				{
					Larticles_Remove(larticles,j);
				}
			}
			if (larticles->larticles[i].time > LARTICLE_TIME)
			{
				if(insight == 1)
				{
					float dangle = atan2(ly - lsee.y, lx - lsee.x);
					
					larticles->larticles[i].potentials[NEURON_SEE_DISTANCE_1] = distance / (float)LARTICLE_VISUAL_RANGE;
					larticles->larticles[i].potentials[NEURON_SEE_DISTANCE_2] = 1.0f / distance;
					larticles->larticles[i].potentials[NEURON_SEE_STATE_1] = 0.0f;
					larticles->larticles[i].potentials[NEURON_SEE_STATE_2] = 0.0f;
					larticles->larticles[i].potentials[NEURON_SEE_STATE_3] = 0.0f;
					if (lsee.state == 1)
					{
						larticles->larticles[i].potentials[NEURON_SEE_STATE_1] = 1.0f;
					}
					if (lsee.state == 2)
					{
						larticles->larticles[i].potentials[NEURON_SEE_STATE_2] = 1.0f;
					}
					if (lsee.state == 3)
					{
						larticles->larticles[i].potentials[NEURON_SEE_STATE_3] = 1.0f;
					}
					larticles->larticles[i].potentials[NEURON_SEE_ANGLE] = dangle - la;
					larticles->larticles[i].potentials[NEURON_SEE_HEALTH] = lsee.health/
					(larticles->larticles[i].health + 1.0f);
					larticles->larticles[i].potentials[NEURON_SEE] = 1.0f;
				}
				else
				{
					larticles->larticles[i].potentials[NEURON_SEE_DISTANCE_1] = 0.0f;
					larticles->larticles[i].potentials[NEURON_SEE_DISTANCE_2] = 0.0f;
					larticles->larticles[i].potentials[NEURON_SEE_STATE_1] = 0.0f;
					larticles->larticles[i].potentials[NEURON_SEE_STATE_2] = 0.0f;
					larticles->larticles[i].potentials[NEURON_SEE_STATE_3] = 0.0f;
					larticles->larticles[i].potentials[NEURON_SEE_ANGLE] = 0.0f;
					larticles->larticles[i].potentials[NEURON_SEE_HEALTH] = 0.0f;
					larticles->larticles[i].potentials[NEURON_SEE] = 0.0f;
				}
			
				larticles->larticles[i].potentials[NEURON_FIELD_DISTANCE] = see_d;
				larticles->larticles[i].potentials[NEURON_FIELD_ANGLE] = see_d;
			}

			float dv = larticles->larticles[i].vx*larticles->larticles[i].vx +
			larticles->larticles[i].vy*larticles->larticles[i].vy;

			if (dv > (float)LARTICLE_MAX_SPEED*(float)LARTICLE_MAX_SPEED)
			{
				float sdv = sqrt(dv);
				larticles->larticles[i].vx = (float)LARTICLE_MAX_SPEED * larticles->larticles[i].vx / sdv;
				larticles->larticles[i].vy = (float)LARTICLE_MAX_SPEED * larticles->larticles[i].vy / sdv;
			}

			larticles->larticles[i].vx *= (1.0f - (float)UNIVERSE_FRICTION);
			larticles->larticles[i].vy *= (1.0f - (float)UNIVERSE_FRICTION);
			larticles->larticles[i].x += larticles->larticles[i].vx;
			larticles->larticles[i].y += larticles->larticles[i].vy;
			if (larticles->larticles[i].health > (float)LARTICLE_MAX_HEALTH)
			{
				larticles->larticles[i].health = LARTICLE_MAX_HEALTH;
			}
		}
		else
		{
			Larticles_Remove(larticles,i);
		}
	}
}
