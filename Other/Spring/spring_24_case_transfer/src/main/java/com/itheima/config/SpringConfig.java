package com.itheima.config;

import org.springframework.context.annotation.ComponentScan;
import org.springframework.context.annotation.Configuration;
import org.springframework.context.annotation.Import;
import org.springframework.context.annotation.PropertySource;
import org.springframework.transaction.annotation.EnableTransactionManagement;

@Configuration
@ComponentScan("com.itheima")
@PropertySource({"classpath:jdbc.properties"})
@Import({JdbcConfig.class , MyBatisConfig.class})
@EnableTransactionManagement // 注解式事务驱动
public class SpringConfig {
}
