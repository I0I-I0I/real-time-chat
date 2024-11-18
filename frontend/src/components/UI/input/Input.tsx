import React, { InputHTMLAttributes, useId } from "react"
import styles from "./Input.module.css"
import cls from "@/utils/cls"

type VariantsType = "default"

interface InputProps extends InputHTMLAttributes<HTMLInputElement> {
	className?: string
	variant?: VariantsType
}

export const Input: React.FC<InputProps> = ({
	className = "",
	variant = "default",
	...props
}) => {
	const id = useId()

	return (
		<input className={cls(styles.input , styles[variant], className)} id={id} {...props} />
	)
}
