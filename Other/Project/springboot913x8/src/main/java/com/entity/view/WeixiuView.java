package com.entity.view;

import com.entity.WeixiuEntity;

import com.baomidou.mybatisplus.annotations.TableName;
import org.apache.commons.beanutils.BeanUtils;
import java.lang.reflect.InvocationTargetException;

import java.io.Serializable;
 

/**
 * 维修
 * 后端返回视图实体辅助类   
 * （通常后端关联的表或者自定义的字段需要返回使用）
 * @author 
 * @email 
 * @date 2030-05-30 20:23:47
 */
@TableName("weixiu")
public class WeixiuView  extends WeixiuEntity implements Serializable {
	private static final long serialVersionUID = 1L;

	public WeixiuView(){
	}
 
 	public WeixiuView(WeixiuEntity weixiuEntity){
 	try {
			BeanUtils.copyProperties(this, weixiuEntity);
		} catch (IllegalAccessException | InvocationTargetException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
 		
	}
}
