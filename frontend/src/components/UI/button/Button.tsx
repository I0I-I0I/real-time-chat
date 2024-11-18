import cls from "@/utils/cls"
import React from "react"
import styles from "./Button.module.css"

type ButtonSize = "default"

type ButtonVariants = "default"

interface ButtonProps extends React.ButtonHTMLAttributes<HTMLButtonElement>  {
	children: string | React.ReactElement
	className?: string
	size?: ButtonSize
	variant?: ButtonVariants
}

export const Button = ({
	children,
	className = "",
	size = "default",
	variant = "default",
}: ButtonProps): JSX.Element => (
	<button
		className={cls(styles.button, styles[size], styles[variant], className)}
	>
		{children}
	</button>
)
