package com.entity.model;

import com.entity.CunxinxiEntity;

import com.baomidou.mybatisplus.annotations.TableName;
import java.util.Date;
import org.springframework.format.annotation.DateTimeFormat;

import com.fasterxml.jackson.annotation.JsonFormat;
import java.io.Serializable;
 

/**
 * 地区信息
 * 接收传参的实体类  
 *（实际开发中配合移动端接口开发手动去掉些没用的字段， 后端一般用entity就够用了） 
 * 取自ModelAndView 的model名称
 * @author 
 * @email 
 * @date 2030-03-31 17:44:51
 */
public class CunxinxiModel  implements Serializable {
	private static final long serialVersionUID = 1L;

	 			
	/**
	 * 地区地址
	 */
	
	private String cundizhi;
		
	/**
	 * 贫困人数
	 */
	
	private String pinkunrenshu;
		
	/**
	 * 联系人
	 */
	
	private String lianxiren;
		
	/**
	 * 联系方式
	 */
	
	private String lianxifangshi;
		
	/**
	 * 地图
	 */
	
	private String ditu;
				
	
	/**
	 * 设置：地区地址
	 */
	 
	public void setCundizhi(String cundizhi) {
		this.cundizhi = cundizhi;
	}
	
	/**
	 * 获取：地区地址
	 */
	public String getCundizhi() {
		return cundizhi;
	}
				
	
	/**
	 * 设置：贫困人数
	 */
	 
	public void setPinkunrenshu(String pinkunrenshu) {
		this.pinkunrenshu = pinkunrenshu;
	}
	
	/**
	 * 获取：贫困人数
	 */
	public String getPinkunrenshu() {
		return pinkunrenshu;
	}
				
	
	/**
	 * 设置：联系人
	 */
	 
	public void setLianxiren(String lianxiren) {
		this.lianxiren = lianxiren;
	}
	
	/**
	 * 获取：联系人
	 */
	public String getLianxiren() {
		return lianxiren;
	}
				
	
	/**
	 * 设置：联系方式
	 */
	 
	public void setLianxifangshi(String lianxifangshi) {
		this.lianxifangshi = lianxifangshi;
	}
	
	/**
	 * 获取：联系方式
	 */
	public String getLianxifangshi() {
		return lianxifangshi;
	}
				
	
	/**
	 * 设置：地图
	 */
	 
	public void setDitu(String ditu) {
		this.ditu = ditu;
	}
	
	/**
	 * 获取：地图
	 */
	public String getDitu() {
		return ditu;
	}
			
}
