package com.controller;

import java.math.BigDecimal;
import java.text.SimpleDateFormat;
import java.text.ParseException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Calendar;
import java.util.Map;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Date;
import java.util.List;
import javax.servlet.http.HttpServletRequest;

import com.utils.ValidatorUtils;
import org.apache.commons.lang3.StringUtils;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.transaction.annotation.Transactional;
import org.springframework.format.annotation.DateTimeFormat;
import org.springframework.web.bind.annotation.PathVariable;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RequestParam;
import org.springframework.web.bind.annotation.RestController;
import com.baomidou.mybatisplus.mapper.EntityWrapper;
import com.baomidou.mybatisplus.mapper.Wrapper;
import com.annotation.IgnoreAuth;

import com.entity.SusheanpaiEntity;
import com.entity.view.SusheanpaiView;

import com.service.SusheanpaiService;
import com.service.TokenService;
import com.utils.PageUtils;
import com.utils.R;
import com.utils.MD5Util;
import com.utils.MPUtil;
import com.utils.CommonUtil;
import java.io.IOException;

/**
 * 宿舍安排
 * 后端接口
 * @author 
 * @email 
 * @date 2030-05-30 20:23:47
 */
@RestController
@RequestMapping("/susheanpai")
public class SusheanpaiController {
    @Autowired
    private SusheanpaiService susheanpaiService;


    


    /**
     * 后端列表
     */
    @RequestMapping("/page")
    public R page(@RequestParam Map<String, Object> params,SusheanpaiEntity susheanpai,
		HttpServletRequest request){
		String tableName = request.getSession().getAttribute("tableName").toString();
		if(tableName.equals("xuesheng")) {
			susheanpai.setXuehao((String)request.getSession().getAttribute("username"));
		}
		if(tableName.equals("susheguanliyuan")) {
			susheanpai.setSuguanzhanghao((String)request.getSession().getAttribute("username"));
		}
        EntityWrapper<SusheanpaiEntity> ew = new EntityWrapper<SusheanpaiEntity>();

		PageUtils page = susheanpaiService.queryPage(params, MPUtil.sort(MPUtil.between(MPUtil.likeOrEq(ew, susheanpai), params), params));

        return R.ok().put("data", page);
    }
    
    /**
     * 前端列表
     */
	@IgnoreAuth
    @RequestMapping("/list")
    public R list(@RequestParam Map<String, Object> params,SusheanpaiEntity susheanpai, 
		HttpServletRequest request){
        EntityWrapper<SusheanpaiEntity> ew = new EntityWrapper<SusheanpaiEntity>();

		PageUtils page = susheanpaiService.queryPage(params, MPUtil.sort(MPUtil.between(MPUtil.likeOrEq(ew, susheanpai), params), params));
        return R.ok().put("data", page);
    }

	/**
     * 列表
     */
    @RequestMapping("/lists")
    public R list( SusheanpaiEntity susheanpai){
       	EntityWrapper<SusheanpaiEntity> ew = new EntityWrapper<SusheanpaiEntity>();
      	ew.allEq(MPUtil.allEQMapPre( susheanpai, "susheanpai")); 
        return R.ok().put("data", susheanpaiService.selectListView(ew));
    }

	 /**
     * 查询
     */
    @RequestMapping("/query")
    public R query(SusheanpaiEntity susheanpai){
        EntityWrapper< SusheanpaiEntity> ew = new EntityWrapper< SusheanpaiEntity>();
 		ew.allEq(MPUtil.allEQMapPre( susheanpai, "susheanpai")); 
		SusheanpaiView susheanpaiView =  susheanpaiService.selectView(ew);
		return R.ok("查询宿舍安排成功").put("data", susheanpaiView);
    }
	
    /**
     * 后端详情
     */
    @RequestMapping("/info/{id}")
    public R info(@PathVariable("id") Long id){
        SusheanpaiEntity susheanpai = susheanpaiService.selectById(id);
        return R.ok().put("data", susheanpai);
    }

    /**
     * 前端详情
     */
	@IgnoreAuth
    @RequestMapping("/detail/{id}")
    public R detail(@PathVariable("id") Long id){
        SusheanpaiEntity susheanpai = susheanpaiService.selectById(id);
        return R.ok().put("data", susheanpai);
    }
    



    /**
     * 后端保存
     */
    @RequestMapping("/save")
    public R save(@RequestBody SusheanpaiEntity susheanpai, HttpServletRequest request){
    	susheanpai.setId(new Date().getTime()+new Double(Math.floor(Math.random()*1000)).longValue());
    	//ValidatorUtils.validateEntity(susheanpai);
        susheanpaiService.insert(susheanpai);
        return R.ok();
    }
    
    /**
     * 前端保存
     */
    @RequestMapping("/add")
    public R add(@RequestBody SusheanpaiEntity susheanpai, HttpServletRequest request){
    	susheanpai.setId(new Date().getTime()+new Double(Math.floor(Math.random()*1000)).longValue());
    	//ValidatorUtils.validateEntity(susheanpai);
        susheanpaiService.insert(susheanpai);
        return R.ok();
    }



    /**
     * 修改
     */
    @RequestMapping("/update")
    @Transactional
    public R update(@RequestBody SusheanpaiEntity susheanpai, HttpServletRequest request){
        //ValidatorUtils.validateEntity(susheanpai);
        susheanpaiService.updateById(susheanpai);//全部更新
        return R.ok();
    }



    

    /**
     * 删除
     */
    @RequestMapping("/delete")
    public R delete(@RequestBody Long[] ids){
        susheanpaiService.deleteBatchIds(Arrays.asList(ids));
        return R.ok();
    }
    
	









}
